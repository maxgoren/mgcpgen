#ifndef actions_hpp
#define actions_hpp
#include <iostream>
#include <stack>
#include "token.hpp"
#include "ast.hpp"
#include "visitors.hpp"
using namespace std;

using Symbol = string;

void buildBinaryOpTree(stack<AST*>& semStack, stack<Symbol>& opStack) {
    AST* rhs = semStack.top(); semStack.pop();
    AST* lhs  = semStack.top(); semStack.pop();
    Symbol op = opStack.top(); opStack.pop();
    cout<<"Building "<<op<<" node"<<endl;
    AST* node = new Binary(op, lhs, rhs);
    semStack.push(node);
    PrintVisitor* pv = new PrintVisitor();
    cout<<"Push: "; 
    pv->visit(dynamic_cast<Binary*>(node));
}

void buildUnaryOpTree(stack<AST*>& semStack, stack<Symbol>& opStack) {
    AST* operand = semStack.top(); semStack.pop();
    Symbol op = opStack.top(); opStack.pop();

    AST* node = new Unary(op, operand);
    semStack.push(node);
}

AST* attachLeaf(AST* node, AST* left) {
    cout<<"\n called attach leaf"<<endl;
    Binary* b = dynamic_cast<Binary*>(node);
    while (b != nullptr) {
        if (b->right == nullptr)
            break;
        b = dynamic_cast<Binary*>(b->right);
    }
    b->right =  b->left;
    b->left = left;
    return node;
}

void sewTogether(stack<AST*>& semStack) {
    AST* right = semStack.top(); semStack.pop();
    AST* left  = semStack.top(); semStack.pop();
    if (right == nullptr)
        semStack.push(left);
    else {
        cout<<"\n called sew together \n";
        right = attachLeaf(right, left);
        semStack.push(right);
    }
}


void buildWhileStatement(stack<AST*>& semStack) {
    cout<<"Assembling While Statement"<<endl;
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
    cout<<"Assembling Print Statement"<<endl;
    AST* left = semStack.top(); semStack.pop();
    PrintStmt* pStmt = dynamic_cast<PrintStmt*>(semStack.top()); semStack.pop();
    pStmt->expr = left;
    semStack.push(pStmt);
}

void buildLetStatement(stack<AST*>& semStack) {
    cout<<"Assembling Let Statement."<<endl;
    AST* left = semStack.top(); semStack.pop();
    LetStmt* lStmt = dynamic_cast<LetStmt*>(semStack.top()); semStack.pop();
    lStmt->expr = left;
    semStack.push(lStmt);
}

void buildProcedureDef(stack<AST*>& semStack) {
    cout<<"Assembling Procedure def"<<endl;
    PrintVisitor* pv = new PrintVisitor();
    Identifier*  name = nullptr;
    ArgsList* args = nullptr;
    StmtSequence* body = nullptr;
    if (dynamic_cast<StmtSequence*>(semStack.top()) != nullptr) {
        body = (StmtSequence*)semStack.top(); semStack.pop();
        pv->visit(body);
    } else {
        cout<<"\n hmm.. "<<endl;
        body = new StmtSequence();
        body->addStmt(semStack.top());
        semStack.pop();
    }
    if (dynamic_cast<ArgsList*>(semStack.top()) != nullptr) {
        args = dynamic_cast<ArgsList*>(semStack.top());
        pv->visit(args);
        semStack.pop();
    } else {
        cout<<"\n gulp..."<<endl;
    }
    if (dynamic_cast<Identifier*>(semStack.top()) != nullptr) {
        name = (Identifier*)semStack.top(); semStack.pop();
        pv->visit(name);
    } else {
        cout<<"\n\n hmm.. wheres el name?"<<endl;
    }
    if (dynamic_cast<DefStmt*>(semStack.top()) != nullptr) {
        DefStmt* def = (DefStmt*) semStack.top(); semStack.pop();
        def->name = name;
        def->args = args;
        def->body = body;
        semStack.push(def);
    } else {
        cout<<"Well, fuck."<<endl;
    }
}

void buildArgsList(stack<AST*>& semStack) {
    cout<<"Constructing arg list"<<endl;
    if (dynamic_cast<LetStmt*>(semStack.top())) {
        LetStmt* arg = dynamic_cast<LetStmt*>(semStack.top()); semStack.pop();
        if (dynamic_cast<ArgsList*>(semStack.top())) {
            dynamic_cast<ArgsList*>(semStack.top())->arguments.push_back(arg);
        } else {
            cout<<"Shit man, that aint an argument list."<<endl;
        }
    }
}

bool isStmt(AST* ast) {
    return (isPrintStmt(ast) || isWhileStmt(ast) || isStmtSequence(ast));
}

void buildStatementSequence(stack<AST*>& semStack) {  
    cout<<"Assembling Statement Sequence."<<endl;
    AST* a = nullptr;
    AST* b = nullptr;
    StmtSequence* ss = nullptr;
    switch (semStack.size()) {
        case 0: cout<<"What in the..."<<endl; return;
        case 1: {
            a = semStack.top(); semStack.pop();
            StmtSequence* ss = new StmtSequence(a);
            semStack.push(ss);
            return;
        } break;
        default:
            break;
    };
    ss = new StmtSequence();
    if (!semStack.empty()) {
        a = semStack.top(); semStack.pop();
    }
    if (!semStack.empty()) {
        b = semStack.top(); semStack.pop();
    }
    if (isExpression(a)) {
        ExprStmt* est = new ExprStmt(a);
        a = est;
    }
    if (isExpression(b)) {
        ExprStmt* est = new ExprStmt(b);
        b = est;
    }
    ss->addStmt(b);
    ss->addStmt(a);
    semStack.push(ss);
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
    cout<<"    -[Applying Rule: "<<id<<"]"<<endl;
    switch(id) {
        case 2: 
            buildStatementSequence(semStack);
            break;
        case 5:
            break;
        case 10:
            semStack.push(nullptr);
            break;
        case 12:
            buildLetStatement(semStack);
            break;
        case 13:
            buildProcedureDef(semStack);
            break;
        case 17:
            buildWhileStatement(semStack);
            break;
        case 19:
            buildPrintStatement(semStack);
            break;
        case 20:
            break;
        case 21:     
            sewTogether(semStack);
            break;
        case 22: // := build assignment ops    
            buildBinaryOpTree(semStack, opStack);
            break;
        case 23:    
            semStack.push(nullptr);
            break;
        case 24:
            sewTogether(semStack);
            break;
        case 25: // <
        case 26: // >  build rel ops
            buildBinaryOpTree(semStack, opStack);
            break;
        case 27:    
            semStack.push(nullptr); 
            break;
        case 28:
            sewTogether(semStack);
            break;
        case 29: // +
        case 30: // -
            buildBinaryOpTree(semStack, opStack);
            break;
        case 31:
            semStack.push(nullptr);
            break;
        case 32:
            sewTogether(semStack);
            break;
        case 33: // *
        case 34: // /
            buildBinaryOpTree(semStack, opStack);
            break;
        case 35:
            semStack.push(nullptr);
            break;
        case 36: //unary
            buildUnaryOpTree(semStack, opStack);
            break;
        case 48:
        case 50:
            buildArgsList(semStack);
            break;
        default:
            break;
    }
}
void handleTerminalSymbols(Symbol X, Token& a, stack<AST*>& semStack, stack<Symbol>& opStack) {
    bool success = true;
    if (X == "TK_NUM") {
        semStack.push(new Number(a.getString()));
    } else if (X == "TK_ID") {
        semStack.push(new Identifier(a.getString()));
    } else if (X == "TK_PRINT") {
        semStack.push(new PrintStmt());
    } else if (X == "TK_WHILE") {
        semStack.push(new WhileStmt());
    } else if (X == "TK_IF") {
        semStack.push(new IfStmt());
    } else if (X == "TK_DEF") {
        semStack.push(new DefStmt());
    } else if (X == "TK_LET") {
        semStack.push(new LetStmt());
    } else if (X == "TK_PLUS" || X == "TK_MINUS" || X == "TK_MUL" || X == "TK_DIV" || X == "TK_LT" || X == "TK_GT" || X == "TK_ASSIGN") {
        opStack.push(a.getString());
    } else if (X == "TK_LPAREN") {
        if (!semStack.empty() && dynamic_cast<Identifier*>(semStack.top()) != nullptr) {
            Identifier* name = (Identifier*)semStack.top(); 
            semStack.pop();
            if (dynamic_cast<DefStmt*>(semStack.top()) == nullptr || dynamic_cast<DefStmt*>(semStack.top()) != nullptr && dynamic_cast<DefStmt*>(semStack.top())->name != nullptr) {
                FuncExpr* fe = new FuncExpr();
                fe->name = name;
                semStack.push(fe);
            } else {
                semStack.push(name);
                semStack.push(new ArgsList());
            }
        }
    } else {
        success = false;
    }
    if (success) {
        cout<<"\t \t \t \tPUSH("<<a.getString()<<")"<<endl;

    }
}

#endif