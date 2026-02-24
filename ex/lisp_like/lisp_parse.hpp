#ifndef recognizer_hpp
#define recognizer_hpp
#include <iostream>
#include <stack>
#include "ast.hpp"
#include "lisp_actions.hpp"
#include "lexer.hpp"
#include "../../src/cfg.hpp"
using namespace std; 

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
        int tpos;
        Token lookahead() {
            return tokens[tpos];
        }
        void advance() {
            tpos++;
        }
        bool match(Symbol sym) {
            if (sym == tokenStr[lookahead().getSymbol()]) {
                advance();
                return true;
            }
            return false;
        }
        void pushProduction(stack<ParseStackSymbol>& st,Production p);
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
    tpos = 0;
    return parseInput(startSymbol);
}

#define MISMATCH 1
#define NO_RULE  2

nullptr_t syntaxError(const Symbol& X, Token& a, int type) {
    switch (type) {
        case MISMATCH: std::cerr << "Error: expected "<< X << " got " << tokenStr[a.getSymbol()] << "\n"; break;
        case NO_RULE: std::cerr << "Error: no rule for M["<< X << "," << tokenStr[a.getSymbol()] << "]\n"; break;
        default: break;
    }
    return nullptr;
}

AST* Parser::parseInput(const Symbol& startSymbol) {
    
    std::stack<ParseStackSymbol> st;
    std::stack<AST*> semStack;
    std::stack<std::string> opStack;
    st.push({NONTERMINAL, GOAL, 0});        // Push end marker
    st.push({NONTERMINAL, startSymbol, 0}); // Push start symbol
    while (!st.empty()) {
        Symbol X = st.top().name;
        Token curr = tokens[tpos];
        int actionId = st.top().actionId;
        cout<<"("<<tpos<<")M ["<<X<<"]["<<tokenStr[curr.getSymbol()]<<"] ("<<curr.getString()<<"), ActionId: "<<actionId<<endl;
        // Accept
        if (X == GOAL && curr.getSymbol() == TK_EOI) {
            cout<<"Accepted with "<<semStack.size()<<", "<<opStack.size()<<" left."<<endl;
            return semStack.top();
        }
        if (st.top().kind == ACTION) {
            actionDispatch(actionId, semStack, opStack);
            st.pop();
        } else if (G.isTerminal(X) || X == GOAL) {
            if (X == tokenStr[curr.getSymbol()]) {
                handleTerminalSymbols(X, curr, semStack, opStack);
                st.pop();
                advance();
            } else {
                return syntaxError(X, curr, MISMATCH);
            }
        } else {
            if (table[X].find(tokenStr[curr.getSymbol()]) == table[X].end()) {
                return syntaxError(X, curr, NO_RULE);
            }
            Production p = table[X][tokenStr[curr.getSymbol()]];
            pushProduction(st, p);
        }
    }
    return nullptr;
}

void Parser::pushProduction(stack<ParseStackSymbol>& st, Production p) {
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

#endif