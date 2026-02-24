#ifndef ast_actions_hpp
#define ast_actions_hpp
#include <iostream>
#include <stack>
#include "token.hpp"
#include "ast.hpp"
using namespace std;

using Symbol = string;


AST* attachLeaf(AST* node, AST* left) {
    Cons* b = dynamic_cast<Cons*>(node);
    while (b->cdr != nullptr) {
        if (b->cdr == nullptr)
            break;
        b = dynamic_cast<Cons*>(b->cdr);
    }
    b->cdr = left;
    return node;
}

void sewTogether(stack<AST*>& semStack) {
    AST* rightChain = semStack.top(); semStack.pop();
    AST* left       = semStack.top(); semStack.pop();
    if (rightChain == nullptr)
        semStack.push(left);
    else {
        rightChain = attachLeaf(rightChain, left);
        semStack.push(rightChain);
    }
}

/*

    you will notice a pattern of the following actions:

        case A: sewTogether(semStack);
        case B:
        case C: buildBinaryOpTree(semStack, opStack);
        case D: semStack.push(nullptr)

    thats operator precedence being built into the grammar.
*/
void actionDispatch(int id, stack<AST*>& semStack, stack<Symbol>& opStack) {
    switch(id) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6: {
            Cons* cons = new Cons();
            if (semStack.size() < 2) {
                cons->car = semStack.top(); semStack.pop();
                semStack.push(cons);
            } else {
                auto a = semStack.top(); semStack.pop();
                auto b = semStack.top(); semStack.pop();
                cons->car = a;
                cons->append(b);
                semStack.push(cons);
            }
        } break;
        default:
            break;
    }
}

bool checkSpecialSymbols(Symbol X) {
    if (X == "TK_PLUS" || X == "TK_MINUS" || X == "TK_MUL" || X == "TK_DIV") 
        return true;
    if(X == "TK_LT" || X == "TK_GT")
        return true;
    if (X == "TK_QUOTE" || X == "TK_LPAREN")
        return true;

    return false;
}

void handleTerminalSymbols(Symbol X, Token& a, stack<AST*>& semStack, stack<Symbol>& opStack) {
    bool success = false;
    if (X == "TK_NUM") {
        semStack.push(new Atom(a.getString()));
        success = true;
    } else if (X == "TK_ID") {
        semStack.push(new Atom(a.getString()));
        success = true;
    } else if (checkSpecialSymbols(X)) {
        semStack.push(new Atom(a.getString()));
        success = true;
    } 
    if (success) {
        cout<<"\t \t \t \tPUSH("<<a.getString()<<")"<<endl;

    }
}

#endif