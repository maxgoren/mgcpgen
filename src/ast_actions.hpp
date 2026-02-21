#ifndef ast_actions_hpp
#define ast_actions_hpp
#include <iostream>
#include <stack>
#include "token.hpp"
#include "ast.hpp"
using namespace std;

using Symbol = string;


void buildBinaryOpTree(stack<AST*>& semStack, stack<Symbol>& opStack) {
    AST* chain = semStack.top(); semStack.pop();
    AST* term  = semStack.top(); semStack.pop();
    Symbol op = opStack.top(); opStack.pop();

    AST* node = new Binary(op, term, chain);
    node->print();
    semStack.push(node);
}

void buildUnaryOpTree(stack<AST*>& semStack, stack<Symbol>& opStack) {
    AST* factor = semStack.top(); semStack.pop();
    Symbol op = opStack.top(); opStack.pop();

    AST* node = new Unary(op, factor);
    semStack.push(node);
    semStack.top()->print();
}

AST* attachLeaf(AST* node, AST* left) {
    Binary* b = dynamic_cast<Binary*>(node);
    while (b->right != nullptr) {
        if (b->right == nullptr)
            break;
        b = dynamic_cast<Binary*>(b->right);
    }
    b->right = b->left;
    b->left = left;
    return b;
}

void sewTogether(stack<AST*>& semStack, stack<Symbol>& opStack) {
    AST* rightChain = semStack.top(); semStack.pop();
    AST* left       = semStack.top(); semStack.pop();
    if (rightChain == nullptr)
        semStack.push(left);
    else {
        rightChain = attachLeaf(rightChain, left);
        semStack.push(rightChain);
    }
}

void actionDispatch(int id, stack<AST*>& semStack, stack<Symbol>& opStack) {
    switch(id) {
        case 1: {    
                    cout<<"\t \t \t EXECUTING: "<<id<<endl;
                    StmtSequence* ss = new StmtSequence();
                    if (!semStack.empty())
                        ss->addStmt(semStack.top());
                    semStack.push(ss); 
                    break;
                }
        case 2: {
            cout<<"\t \t \t EXECUTING: "<<id<<endl;
            if (!semStack.empty()) {
                cout<<"Adding statement to sequence"<<endl;
                auto stmt = semStack.top(); semStack.pop();
                stmt->print();
                ((StmtSequence*)(semStack.top()))->addStmt(stmt);
            }
            break;
        }
        case 7: {
            cout<<"\t \t \t EXECUTING: "<<id<<endl;
            cout<<"print-stmtT -> expr"<<endl;
            AST* left = semStack.top(); semStack.pop();
            semStack.push(new PrintStmt(left));
            break;
        }
        // exp → term expT
        case 9: {
            cout<<"\t \t \t EXECUTING: "<<id<<endl;
            cout<<"exp → term expT"<<endl;
            sewTogether(semStack, opStack);
            break;
        }

        // expT → addop term expT
        case 10:
        case 11: 
            cout<<"\t \t \tEXECUTING: "<<id<<endl;
            cout<<"expT → addop term expT"<<endl;
            buildBinaryOpTree(semStack, opStack);
            break;

        // expT → ε
        case 12:    cout<<"\t \t EXECUTING: "<<id<<endl;

            semStack.push(nullptr);
            break;

        // term → factor termT
        case 13:     cout<<"\t \t EXECUTING: "<<id<<endl;

            sewTogether(semStack, opStack);
            break;

        // termT → mulop factor termT
        case 14:
        case 15:     cout<<"\t \t EXECUTING: "<<id<<endl;

            cout<<"termT → mulop factor termT"<<endl;
            buildBinaryOpTree(semStack, opStack);
            break;
        

        // termT → ε
        case 16:    cout<<"\t \t EXECUTING: "<<id<<endl;

            semStack.push(nullptr);
            break;

        // factor → TK_MINUS factor (unary minus)
        case 17:    cout<<"\t \t EXECUTING: "<<id<<endl;

            buildUnaryOpTree(semStack, opStack);
            break;
        
        default:
            break;
    }
}

#endif