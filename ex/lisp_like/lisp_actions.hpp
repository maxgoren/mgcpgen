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
        case 5: semStack.push(nullptr); break;
        case 2: {
            Cons* nl = new Cons();;
            while (!semStack.empty()) {
                auto p = dynamic_cast<Atom*>(semStack.top());
                if (p != nullptr && p->data == "(") {
                    break;
                } 
                nl->append(semStack.top()); semStack.pop();
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