#ifndef actions_hpp
#define actions_hpp
#include <stack>
#include <iostream>
#include "token.hpp"
using std::stack;
using std::string;

using Symbol = string;

bool isBinaryOp(int id) {
        //    :=         ==          !=            <         >            +           -            *           /
    return (id == 5 || id == 8 || id == 9 || id == 12 || id == 13 || id == 16 || id == 17 || id == 20 || id == 21);
}

bool isUnaryOp(int id) {
    return (id == 23);
}

void actionDispatch(int id, stack<AST*>& semStack, stack<Symbol>& opStack) {
    cout<<"Apply action: "<<id<<endl;
    if (isBinaryOp(id)) {
        AST* r = semStack.top(); semStack.pop();
        AST* l = semStack.top(); semStack.pop();
        Symbol op = opStack.top(); opStack.pop();
        semStack.push(new Binary(op, l, r));
    } else if (isUnaryOp(id)) {
        AST* e = semStack.top(); semStack.pop();
        semStack.push(new Unary(opStack.top(), e));
        opStack.pop();
    } else {
        cout<<"Well this awkward ya see... but whats a "<<id<<" supposed to do?"<<endl;
    }
}

void handleTerminalSymbols(Symbol X, Token& a, stack<AST*>& semStack, stack<Symbol>& opStack) {
    if (X == "TK_ID") {
        semStack.push(new Identifier(a.getString()));
    } else if (X == "TK_NUM") {
        cout<<"PUSH: "<<a.getString()<<endl;
    } else if (X == "TK_PLUS" || X == "TK_MINUS" || X == "TK_MUL" || X == "TK_DIV" || X == "TK_LT" || X == "TK_GT" || X == "TK_ASSIGN" || X == "TK_EQU" || X == "TK_NEQ") {
        opStack.push(a.getString());
    }
}

#endif