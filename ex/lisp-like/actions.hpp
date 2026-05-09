#ifndef actions_hpp
#define actions_hpp
#include <stack>
#include <iostream>
#include "token.hpp"
using std::stack;
using std::string;

using Symbol = string;


void actionDispatch(int id, stack<AST*>& semStack, stack<Symbol>& opStack) {
    cout<<"\n\t[D: "<<id<<"]\n"<<endl;
    switch (id) {
        case 3: {
            vector<AST*> elem;
            while (!semStack.empty() && dynamic_cast<List*>(semStack.top()) == nullptr) {
                elem.push_back(semStack.top());
                semStack.pop();
            }
            dynamic_cast<List*>(semStack.top())->elements = elem;
        }
    }
}

void handleTerminalSymbols(Symbol X, Token& a, stack<AST*>& semStack, stack<Symbol>& opStack) {
    switch (a.getSymbol()) {
        case TK_LPAREN: semStack.push(new List()); break;
        case TK_NUMBER: 
        case TK_ID:     
        case TK_OP:     semStack.push(new Atom(a)); break;
        case TK_QUOTE:
        case TK_RPAREN:
        default:
            break;
    }
}

#endif