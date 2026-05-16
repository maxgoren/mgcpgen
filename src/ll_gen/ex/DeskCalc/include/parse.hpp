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
        std::stack<ParseStackSymbol> st;
        std::stack<AST*>             semStack;
        std::stack<Symbol>           opStack;
        Grammar       G;
        ParseTable    table;
        set<Symbol>   terms;
        set<Symbol>   nonterms;
        vector<Token> tokens;
        int           ipos;
        AST* parseInput(const Symbol& startSymbol);
        void handleNonTerminal(Symbol X, Token& curr);
        void printState(Symbol X, Token& curr, int aid);
        Token current();
        void advance();
    public:
        Parser(ParseTable& pt, set<Symbol>& ts, set<Symbol>& nts);
        AST* parse(vector<Token>& token, const Symbol& startSymbol);
};

Parser::Parser(ParseTable& pt,  set<Symbol>& ts, set<Symbol>& nts) {
    terms = ts;
    nonterms = nts;
    table = pt;
}

Token Parser::current() {
    return tokens[ipos];
}

void Parser::advance() {
    ipos++;
}

AST* Parser::parse(vector<Token>& token, const Symbol& startSymbol) {
    tokens = token;
    ipos = 0;
    return parseInput(startSymbol);
}

#define MISMATCH 1
#define NO_RULE  2

nullptr_t syntaxError(const Symbol& X, Token& curr, int type) {
    switch (type) {
        case MISMATCH: std::cerr << "Error: expected "<< X << " got " << tokenStr[curr.getSymbol()] << "\n"; break;
        case NO_RULE: std::cerr << "Error: no rule for M["<< X << "," << tokenStr[curr.getSymbol()] << "]\n"; break;
        default: break;
    }
    return nullptr;
}

void Parser::printState(Symbol X, Token& curr, int actionId) {
    cout<<"("<<ipos<<")M ["<<X<<"]["<<tokenStr[curr.getSymbol()]<<"] ("<<curr.getString()<<"), ActionId: "<<actionId<<endl;
}


AST* Parser::parseInput(const Symbol& startSymbol) {
    // Push end marker && start symbol
    st.push({NONTERMINAL, GOAL, 0});
    st.push({NONTERMINAL, startSymbol, 0});
    while (!st.empty()) {
        Symbol X = st.top().name;
        Token curr = current();
        if (st.top().kind != ACTION) {
            printState(X, curr, st.top().actionId);
        }
        if (X == GOAL && curr.getSymbol() == TK_EOI) {
            cout<<"Accepted with "<<semStack.size()<<", "<<opStack.size()<<" left."<<endl;
            return semStack.empty() ? nullptr:semStack.top();
        }
        if (st.top().kind == ACTION) {
            actionDispatch(st.top().actionId, semStack, opStack);
            st.pop();
        } else if (st.top().kind == TERMINAL || X == GOAL) {
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
            handleNonTerminal(X, curr);
        }
    }
    return nullptr;
}

void Parser::handleNonTerminal(Symbol X, Token& curr) {
    Production p = table[X][tokenStr[curr.getSymbol()]];
    st.pop();
    // push RHS in reverse order
    for (auto it = p.rhs.rbegin();  it != p.rhs.rend(); ++it)  {
        if (*it != EPS && *it == ACTSYM) {
            st.push(ParseStackSymbol(ACTION, "", p.pid));
        } else {
            if (*it != EPS)
                st.push(ParseStackSymbol(symbolKind(terms, nonterms, *it),*it,p.pid));
        }
    }
}

#endif