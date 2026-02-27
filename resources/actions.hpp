#ifndef actions_hpp
#define actions_hpp
#include <stack>
#include <iostream>
#include "token.hpp"
using std::stack;
using std::string;

using Symbol = string;

struct AST {
    virtual ~AST() = default;
    virtual void print() = 0;
    virtual int eval() = 0;
};

void actionDispatch(int id, stack<AST*>& semStack, stack<Symbol>& opStack) {

}

void handleTerminalSymbols(Symbol X, Token& a, stack<AST*>& semStack, stack<Symbol>& opStack) {

}

#endif