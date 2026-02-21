#ifndef recognizer_hpp
#define recognizer_hpp
#include <iostream>
#include <stack>
#include "ast.hpp"
#include "ast_actions.hpp"
#include "lexer.hpp"
#include "cfg.hpp"
using namespace std; 

enum Kind { TERMINAL, NONTERMINAL, ACTION };

Kind symbolKind(Grammar& G, Symbol sym) {
    if (G.isTerminal(sym)) 
        return TERMINAL;
    if (G.isNonTerminal(sym))
        return NONTERMINAL;
    return ACTION;
}

struct StackSymbol {
    Kind kind;
    std::string name;   // for terminal / nonterminal
    int actionId;       // for ACTION
    StackSymbol(Kind k, Symbol s, int aid) : kind(k), name(s), actionId(aid) { }
    StackSymbol() { }
};

string tokenStr[] = { "TK_NUM", "TK_PLUS", "TK_MINUS", "TK_MUL", "TK_DIV", "TK_LPAREN", "TK_RPAREN", "TK_SEMI", "TK_PRINT", "TK_ID", "TK_EOI"};

AST* parseInput(const std::vector<Token>& tokens, Grammar& G, std::map<Symbol, std::map<Symbol, Production>>& table, const Symbol& startSymbol) {
    
    std::stack<StackSymbol> st;
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
        } else if (find(G.terminals.begin(), G.terminals.end(), X) != G.terminals.end() || X == GOAL) {
        // Terminal or end marker
            if (X == tokenStr[a.getSymbol()]) {
                if (X == "TK_NUM") {
                    cout<<"\t \t \t PUSH("<<a.getString()<<")"<<endl;
                    semStack.push(new Number(a.getString()));
                } else if (X == "TK_ID") {
                    cout<<"\t \t \t PUSH("<<a.getString()<<")"<<endl;
                    semStack.push(new Identifier(a.getString()));
                } else if (X == "TK_PLUS" || X == "TK_MINUS" || X == "TK_MUL" || X == "TK_DIV") {
                    cout<<"\t \t \t PUSH("<<a.getString()<<")"<<endl;
                    opStack.push(a.getString());
                }
                st.pop();
                i++;        // consume token
            } else {
                std::cerr << "Error: expected "
                          << X << " got "
                          << tokenStr[a.getSymbol()] << "\n";
                return nullptr;
            }
        } else {
            if (table[X].find(tokenStr[a.getSymbol()]) == table[X].end()) {
                std::cerr << "Error: no rule for M["
                          << X << "," << tokenStr[a.getSymbol()] << "]\n";
                return nullptr;
            }

            Production p = table[X][tokenStr[a.getSymbol()]];
            st.pop();
            //Push Action symbol for this production before RHS
            st.push(StackSymbol(ACTION, "", p.pid));

            // push RHS in reverse order
            for (auto it = p.rhs.rbegin();  it != p.rhs.rend(); ++it)  {
                if (*it != EPS) {
                    st.push(StackSymbol(symbolKind(G, *it),*it,p.pid));
                }
            }
        }
    }
    return semStack.top(); // should never reach
}

#endif