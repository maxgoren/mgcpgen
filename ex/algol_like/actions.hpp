#ifndef ast_actions_hpp
#define ast_actions_hpp
#include <iostream>
#include <stack>
#include "token.hpp"
#include "ast.hpp"
using namespace std;

using Symbol = string;


void buildBinaryOpTree(stack<AST*>& semStack, stack<Symbol>& opStack) {
    AST* rhs = semStack.top(); semStack.pop();
    AST* lhs  = semStack.top(); semStack.pop();
    Symbol op = opStack.top(); opStack.pop();

    AST* node = new Binary(op, lhs, rhs);
    semStack.push(node);
}

void buildUnaryOpTree(stack<AST*>& semStack, stack<Symbol>& opStack) {
    AST* operand = semStack.top(); semStack.pop();
    Symbol op = opStack.top(); opStack.pop();

    AST* node = new Unary(op, operand);
    semStack.push(node);
}

AST* attachLeaf(AST* node, AST* left) {
    Binary* b = dynamic_cast<Binary*>(node);
    while (b != nullptr && b->right != nullptr) {
        if (b->right == nullptr)
            break;
        b = dynamic_cast<Binary*>(b->right);
    }
    b->right = b->left;
    b->left = left;
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

void buildWhileStatement(stack<AST*>& semStack) {
    if (semStack.size() >= 2) {
        AST* body = semStack.top(); semStack.pop();
        AST* test = semStack.top(); semStack.pop();
        WhileStmt* ws = dynamic_cast<WhileStmt*>(semStack.top()); semStack.pop();
        if (ws != nullptr) {
        ws->body = body;
        ws->testExpr = test;
        semStack.push(ws);  
        } else {
            cout<<"Error: was expecting while stmt node."<<endl;
        }  
    }
}

void buildPrintStatement(stack<AST*>& semStack) {
    AST* left = semStack.top(); semStack.pop();
    PrintStmt* pStmt = dynamic_cast<PrintStmt*>(semStack.top()); semStack.pop();
    pStmt->expr = left;
    semStack.push(pStmt);
}

bool isStmt(AST* ast) {
    return (isPrintStmt(ast) || isWhileStmt(ast) || isStmtSequence(ast));
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
        case 2: {    
            auto a = semStack.top(); semStack.pop();
            auto b = semStack.top(); semStack.pop();
            StmtSequence* ss = new StmtSequence();
            ss->addStmt(a);
            ss->addStmt(b);
            semStack.push(ss);
        } break;
        case 7:
            buildPrintStatement(semStack);
            break;
        case 9:
            sewTogether(semStack);
            break;
        case 10:
        case 11: 
            buildBinaryOpTree(semStack, opStack);
            break;
        case 12:    
            semStack.push(nullptr);
            break;
        case 13:     
            sewTogether(semStack);
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
        case 25: 
            buildWhileStatement(semStack);
            break;
        case 26: 
            sewTogether(semStack);
        break;
        case 27:
        case 28:
            buildBinaryOpTree(semStack, opStack);
            break;
        case 29:
            semStack.push(nullptr);
            break;
        case 30:
            sewTogether(semStack);
        break;
        case 31:
            buildBinaryOpTree(semStack, opStack);
            break;
        case 32:
            semStack.push(nullptr);
            break;
        default:
            break;
    }
}
void handleTerminalSymbols(Symbol X, Token& a, stack<AST*>& semStack, stack<Symbol>& opStack) {
    bool success = false;
    if (X == "TK_NUM") {
        semStack.push(new Number(a.getString()));
        success = true;
    } else if (X == "TK_ID") {
        semStack.push(new Identifier(a.getString()));
        success = true;
    } else if (X == "TK_PRINT") {
        semStack.push(new PrintStmt());
        success = true;
    } else if (X == "TK_WHILE") {
        semStack.push(new WhileStmt());
        success = true;
    } else if (X == "TK_PLUS" || X == "TK_MINUS" || X == "TK_MUL" || X == "TK_DIV" || X == "TK_LT" || X == "TK_GT" || X == "TK_ASSIGN") {
        opStack.push(a.getString());
        success = true;
    } 
    if (success) {
        cout<<"\t \t \t \tPUSH("<<a.getString()<<")"<<endl;

    }
}

#endif