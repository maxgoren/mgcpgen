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
    b->print();
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
        case 2: {    
            cout<<"\t \t \t EXECUTING: "<<id<<endl;
            StmtSequence* ss = new StmtSequence();
            if (!semStack.empty())
                ss->addStmt(semStack.top()), semStack.pop();
            semStack.push(ss); 
            break;
        }
        case 7: {
            cout<<"\t \t \t EXECUTING: "<<id<<endl;
            cout<<"print-stmtT -> expr"<<endl;
            AST* left = semStack.top(); semStack.pop();
            PrintStmt* pStmt = (PrintStmt*)semStack.top(); semStack.pop();
            pStmt->expr = left;
            semStack.push(pStmt);
            break;
        }
        case 9: {
            cout<<"\t \t \t EXECUTING: "<<id<<endl;
            cout<<"exp → term expT"<<endl;
            sewTogether(semStack, opStack);
            break;
        }

        case 10:
        case 11: 
            cout<<"\t \t \tEXECUTING: "<<id<<endl;
            cout<<"expT → addop term expT"<<endl;
            buildBinaryOpTree(semStack, opStack);
            break;

        case 12:    
            cout<<"\t \t EXECUTING: "<<id<<"\nexpT → ε"<<endl;
            semStack.push(nullptr);
            break;

        case 13:     
            cout<<"\t \t EXECUTING: "<<id<<"\nterm → factor termT"<<endl;
            sewTogether(semStack, opStack);
            break;

        case 14:
        case 15:     
            cout<<"\t \t EXECUTING: "<<id<<endl;
            cout<<"termT → mulop factor termT"<<endl;
            buildBinaryOpTree(semStack, opStack);
            break;
        
        case 16:    
            cout<<"\t \t EXECUTING: "<<id<<"\ntermT → ε"<<endl;
            semStack.push(nullptr);
            break;
        
        case 17:    
            cout<<"\t \t EXECUTING: "<<id<<"\nfactor → TK_MINUS factor (unary minus)"<<endl;
            buildUnaryOpTree(semStack, opStack);
            break;
        case 25: {
            cout<<"SemStack size: "<<semStack.size()<<endl;
            AST* body = semStack.top(); semStack.pop();
            AST* test = semStack.top(); semStack.pop();
            WhileStmt* ws = (WhileStmt*)semStack.top(); semStack.pop();
            ws->body = body;
            ws->testExpr = test;
            semStack.push(ws);
        }
            break;
        case 26: {
            sewTogether(semStack, opStack);
        } break;
        case 27:
        case 28: {
            cout<<"\t \t EXECUTING: "<<id<<endl;
            cout<<"termT → mulop factor termT"<<endl;
            buildBinaryOpTree(semStack, opStack);
            break;
        }
        case 29: {
            semStack.push(nullptr);
            break;
        }
        default:
            break;
    }
}

#endif