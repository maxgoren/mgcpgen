#ifndef visitors_hpp
#define visitors_hpp
#include "ast.hpp"
#include <iostream>
#include <stack>
#include <map>
#include <unordered_map>
using namespace std;
struct EvalVisitor : Visitor {
    stack<int> sv;
    unordered_map<string, int> valTable;
    map<string, DefStmt*> procedures;
    void visit(Number* n) {
        sv.push(stoi(n->value));
    }  
    void visit(Identifier* n) {
        sv.push(valTable[n->name]);
    }
    void visit(Binary* bexpr) {
        bexpr->left->accept(this);
        bexpr->right->accept(this);
        int rhs = sv.top(); sv.pop();
        int lhs = sv.top(); sv.pop();
        cout<<"lhs: "<<lhs<<endl;
        cout<<"rhs: "<<rhs<<endl;
        cout<<"opr: "<<bexpr->op[0]<<endl;
        switch (bexpr->op[0]) {
            case '+': sv.push(lhs + rhs); break;
            case '-': sv.push(lhs - rhs); break;
            case '/': sv.push(lhs / rhs); break;
            case '*': sv.push(lhs * rhs); break;
            case '<': sv.push(lhs < rhs); break;
            case '>': sv.push(lhs > rhs); break;
            case ':': if (bexpr->op == ":=") {
                valTable[((Identifier*)bexpr->left)->name] = rhs;
                sv.push(rhs);
            } break;
        }
        cout<<"{"<<sv.top()<<"|}";
        cout<<endl;
    }
    void visit(Unary* uexpr) {
        uexpr->left->accept(this);
        sv.top() = -sv.top();
    }
    void visit(PrintStmt* ps) {
        ps->expr->accept(this);
        cout<<sv.top()<<endl;
        sv.pop();
    }
    void visit(WhileStmt* ws) {
        ws->testExpr->accept(this);
        while (sv.top()) {
            sv.pop();
            ws->body->accept(this);
            ws->testExpr->accept(this);
        }
    }
    void visit(LetStmt* ls) {
        ls->expr->accept(this);
    }
    void visit(IfStmt* is) {
        is->testExpr->accept(this); 
        bool result = sv.top(); sv.pop();
        result ? 
            is->truePath->accept(this)
            :
            is->falsePath->accept(this);
    }
    void visit(StmtSequence* ss) {
        if (ss->stmt != nullptr) {
            ss->stmt->accept(this);
        }
        if (ss->next != nullptr) {
            ss->next->accept(this);
        }
    }
    void visit(ExprStmt* es) { es->expr->accept(this); }
    void visit(DefStmt* ds) {
        procedures[ds->name->name] = ds;
    }
    void visit(FuncExpr* expr) {
        DefStmt* ds = procedures[expr->name->name];
        ds->body->accept(this);
    }

};
struct PrintVisitor : Visitor {
    int depth;
    void enter() {
        depth++;
    }
    void leave() {
        depth--;
    }
    void print(string msg) {
        for (int i = 0; i < depth; i++) cout<<" ";
        cout<<msg<<endl;
    }
    PrintVisitor() {
        depth = 0;
    }
    void visit(Number* n) { enter(); print("Num: " + n->value); leave(); }
    void visit(Identifier* n) { enter(); print("ID: " + n->name); leave();   }
    void visit(ExprStmt* es) { es->expr->accept(this); }
    void visit(Binary* bexpr) {
        enter();
        print("Op: " + bexpr->op);
        bexpr->left->accept(this);
        bexpr->right->accept(this);
        leave();
    }
    void visit(Unary* uexpr) {
        enter();
        print("Op: " + uexpr->op);
        uexpr->left->accept(this);
        leave();
    }
    void visit(PrintStmt* ps) {
        enter();
        print("PrintStmt: ");
        ps->expr->accept(this);
        leave();
    }
    void visit(WhileStmt* ws) {
        enter();
        print("While Stmnt: ");
        ws->testExpr->accept(this);
        ws->body->accept(this);
        leave();
    }
    void visit(IfStmt* is) {
        enter();
        print("if stmt ");
        is->testExpr->accept(this);
        is->truePath->accept(this);
        leave();
    }
    void visit(LetStmt* ls) {
        enter();
        print("Let stmt");
        ls->expr->accept(this);
        leave();
    }
    void visit(StmtSequence* ss) {
        if (ss->stmt != nullptr) {
            ss->stmt->accept(this);
        }
        if (ss->next != nullptr) {
            ss->next->accept(this);
        }
    }
    void visit(DefStmt* ds) {
        enter();
        print("Procedure Def");
        ds->name->accept(this);
        ds->body->accept(this);
        leave();
    }
    void visit(FuncExpr* fe) {
        enter();
        print("Function call");
        fe->name->accept(this);
        leave();
    }
};

#endif