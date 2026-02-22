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
            StmtSequence* ss = new StmtSequence();
            if (!semStack.empty())
                ss->addStmt(semStack.top()), semStack.pop();
            semStack.push(ss); 
            break;
        }
        case 7: {
            AST* left = semStack.top(); semStack.pop();
            PrintStmt* pStmt = (PrintStmt*)semStack.top(); semStack.pop();
            pStmt->expr = left;
            semStack.push(pStmt);
            break;
        }
        case 9: {
            sewTogether(semStack, opStack);
            break;
        }
        case 10:
        case 11: 
            buildBinaryOpTree(semStack, opStack);
            break;

        case 12:    
            semStack.push(nullptr);
            break;

        case 13:     
            sewTogether(semStack, opStack);
            break;

        case 14:
        case 15:     
            buildBinaryOpTree(semStack, opStack);
            break;
        
        case 16:    
            semStack.push(nullptr);
            break;
        
        case 17:    
            buildUnaryOpTree(semStack, opStack);
            break;
        case 25: {
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
            buildBinaryOpTree(semStack, opStack);
            break;
        }
        case 29: {
            semStack.push(nullptr);
            break;
        }
        case 30: {
            sewTogether(semStack, opStack);
        } break;
        case 31: {
            buildBinaryOpTree(semStack, opStack);
            break;
        }
        case 32: {
            semStack.push(nullptr);
            break;
        }
        default:
            break;
    }
}
void handleTerminalSymbols(Symbol X, Token& a, stack<AST*>& semStack, stack<Symbol>& opStack) {
    cout<<"\t \t \t PUSH("<<a.getString()<<")"<<endl;
    if (X == "TK_NUM") {
        semStack.push(new Number(a.getString()));
    } else if (X == "TK_ID") {
        semStack.push(new Identifier(a.getString()));
    } else if (X == "TK_PRINT") {
        semStack.push(new PrintStmt());
    } else if (X == "TK_WHILE") {
        semStack.push(new WhileStmt());
    } else if (X == "TK_PLUS" || X == "TK_MINUS" || X == "TK_MUL" || X == "TK_DIV" || X == "TK_LT" || X == "TK_GT" || X == "TK_ASSIGN") {
        opStack.push(a.getString());
    } 
}

#endif