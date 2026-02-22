#ifndef recognizer_hpp
#define recognizer_hpp
#include <iostream>
#include <stack>
#include "ast.hpp"
#include "actions.hpp"
#include "lexer.hpp"
#include "../../src/cfg.hpp"
using namespace std; 


string tokenStr[] = { 
    "TK_NUM", "TK_PLUS", "TK_MINUS", "TK_MUL", "TK_DIV", "TK_LPAREN",
    "TK_RPAREN", "TK_LT", "TK_GT", "TK_LCURLY", "TK_RCURLY", "TK_SEMI", 
    "TK_ASSIGN", "TK_PRINT", "TK_WHILE", "TK_ID", "TK_EOI"
 };

enum StackItemType { TERMINAL, NONTERMINAL, ACTION };

StackItemType symbolKind(Grammar& G, Symbol sym) {
    if (G.isTerminal(sym)) 
        return TERMINAL;
    if (G.isNonTerminal(sym))
        return NONTERMINAL;
    return ACTION;
}

struct ParseStackSymbol {
    StackItemType kind;
    std::string name;   // for terminal / nonterminal
    int actionId;       // for ACTION
    ParseStackSymbol(StackItemType k, Symbol s, int aid) : kind(k), name(s), actionId(aid) { }
    ParseStackSymbol() { }
};

class Parser {
    private:
        Grammar G;
        ParseTable table;
        vector<Token> tokens;
        int i;
        AST* parseInput(const Symbol& startSymbol);
    public:
        Parser(Grammar& gram, ParseTable& pt);
        AST* parse(vector<Token>& token, const Symbol& startSymbol);
};

Parser::Parser(Grammar& gram, ParseTable& pt) {
    G = gram;
    table = pt;
}

AST* Parser::parse(vector<Token>& token, const Symbol& startSymbol) {
    tokens = token;
    i = 0;
    return parseInput(startSymbol);
}

AST* Parser::parseInput(const Symbol& startSymbol) {
    
    std::stack<ParseStackSymbol> st;
    std::stack<AST*> semStack;
    std::stack<std::string> opStack;
    // Push end marker
    st.push({NONTERMINAL, GOAL, 0});
    // Push start symbol
    st.push({NONTERMINAL, startSymbol, 0});

    size_t i = 0; // input pointer

    while (!st.empty()) {

        Symbol X = st.top().name;
        Token a = tokens[i];
        int actionId = st.top().actionId;
        cout<<"("<<i<<")parse stack: "<<X<<", "<<"input token: <"<<tokenStr[a.getSymbol()]<<", "<<a.getString()<<">"<<endl;
        cout<<"("<<i<<")ActionId: "<<actionId<<endl;
        // Accept
        if (X == GOAL && a.getSymbol() == TK_EOI) {
            cout<<"Accepted."<<endl;
            return semStack.top();
        }
        if (st.top().kind == ACTION) {
            actionDispatch(actionId, semStack, opStack);
            st.pop();
        } else if (G.isTerminal(X) || X == GOAL) {
            if (X == tokenStr[a.getSymbol()]) {
                handleTerminalSymbols(X, a, semStack, opStack);
                st.pop();
                i++;        // consume token
            } else {
                std::cerr << "Error: expected "<< X << " got " << tokenStr[a.getSymbol()] << "\n";
                return nullptr;
            }
        } else {
            if (table[X].find(tokenStr[a.getSymbol()]) == table[X].end()) {
                std::cerr << "Error: no rule for M["<< X << "," << tokenStr[a.getSymbol()] << "]\n";
                return nullptr;
            }

            Production p = table[X][tokenStr[a.getSymbol()]];
            st.pop();
            //Push Action symbol for this production before RHS
            st.push(ParseStackSymbol(ACTION, "", p.pid));

            // push RHS in reverse order
            for (auto it = p.rhs.rbegin();  it != p.rhs.rend(); ++it)  {
                if (*it != EPS) {
                    st.push(ParseStackSymbol(symbolKind(G, *it),*it,p.pid));
                }
            }
        }
    }
    return semStack.top();
}

#endif