#ifndef parser_hpp
#define parser_hpp
#include <iostream>
#include <stack>
#include <set>
#include "ast.hpp"
#include "actions.hpp"
#include "lexer.hpp"
#include "cfg.hpp"
using namespace std; 


enum StackItemType { TERMINAL, NONTERMINAL, ACTION };

bool isTerminal(set<Symbol>& terminals, Symbol X) {
    return terminals.find(X) != terminals.end();
}

bool isNonTerminal(set<Symbol>& nonterminals, Symbol X) {
    return nonterminals.find(X) != nonterminals.end();
}


StackItemType symbolKind(set<Symbol>& terminalSymbols, set<Symbol>& nonTerminalSymbols, Symbol sym) {
    if (isTerminal(terminalSymbols, sym)) 
        return TERMINAL;
    if (isNonTerminal(nonTerminalSymbols, sym))
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

using ParseTable = map<Symbol, map<Symbol, Production>>;

class Parser {
    private:
        Grammar G;
        ParseTable table;
        set<Symbol> terms;
        set<Symbol> nonterms;
        vector<Token> tokens;
        int i;
        AST* parseInput(const Symbol& startSymbol);
        void printState(int tokenNum, Symbol X, Token& a, int aid);
    public:
        Parser(ParseTable& pt, set<Symbol>& ts, set<Symbol>& nts);
        AST* parse(vector<Token>& token, const Symbol& startSymbol);
};

Parser::Parser(ParseTable& pt,  set<Symbol>& ts, set<Symbol>& nts) {
    terms = ts;
    nonterms = nts;
    table = pt;
}

AST* Parser::parse(vector<Token>& token, const Symbol& startSymbol) {
    tokens = token;
    i = 0;
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

void Parser::printState(int tokenNum, Symbol X, Token& a, int actionId) {
    cout<<"("<<tokenNum<<")M ["<<X<<"]["<<tokenStr[a.getSymbol()]<<"] ("<<a.getString()<<"), ActionId: "<<actionId<<endl;
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
        if (st.top().kind != ACTION) {
            printState(i, X, a, actionId);
        }
        // Accept
        if (X == GOAL && a.getSymbol() == TK_EOI) {
            cout<<"Accepted with "<<semStack.size()<<", "<<opStack.size()<<" left."<<endl;
            return semStack.empty() ? nullptr:semStack.top();
        }
        if (st.top().kind == ACTION) {
            actionDispatch(actionId, semStack, opStack);
            st.pop();
        } else if (isTerminal(terms, X) || X == GOAL) {
            if (X == tokenStr[a.getSymbol()]) {
                handleTerminalSymbols(X, a, semStack, opStack);
                st.pop();
                i++;        // consume token
            } else {
                return syntaxError(X, a, MISMATCH);
            }
        } else {
            if (table[X].find(tokenStr[a.getSymbol()]) == table[X].end()) {
                return syntaxError(X, a, NO_RULE);
            }
            Production p = table[X][tokenStr[a.getSymbol()]];
            st.pop();
            // push RHS in reverse order
            for (auto it = p.rhs.rbegin();  it != p.rhs.rend(); ++it)  {
                if (*it != EPS) {
                    if (*it == ACTSYM) {
                        st.push(ParseStackSymbol(ACTION, "", p.pid));
                    } else {
                        st.push(ParseStackSymbol(symbolKind(terms, nonterms, *it),*it,p.pid));
                    }
                }
            }
        }
    }
    return nullptr;
}

#endif