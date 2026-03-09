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
    unordered_map<string,int> valTable;
    void eval(AST* ast) {
        if (dynamic_cast<Number*>(ast)) {
            visit(dynamic_cast<Number*>(ast));
        } else if (dynamic_cast<Identifier*>(ast)) {
            visit(dynamic_cast<Identifier*>(ast));
        } else if (dynamic_cast<Binary*>(ast)) {
            visit(dynamic_cast<Binary*>(ast));
        } else if (dynamic_cast<Unary*>(ast)) {
            visit(dynamic_cast<Unary*>(ast));
        }
    }
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
        cout<<"{ "<<lhs<<" "<<bexpr->op<<" "<<rhs<<" := "<<"["<<sv.top()<<"] }"<<endl;
    }
    void visit(Unary* uexpr) {
        uexpr->left->accept(this);
        sv.top() = -sv.top();
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
    void print(AST* ast) {
        if (dynamic_cast<Number*>(ast)) {
            visit(dynamic_cast<Number*>(ast));
        } else if (dynamic_cast<Identifier*>(ast)) {
            visit(dynamic_cast<Identifier*>(ast));
        } else if (dynamic_cast<Binary*>(ast)) {
            visit(dynamic_cast<Binary*>(ast));
        } else if (dynamic_cast<Unary*>(ast)) {
            visit(dynamic_cast<Unary*>(ast));
        }
    }
    void visit(Number* n) { enter(); print("Num: " + n->value); leave(); }
    void visit(Identifier* n) { enter(); print("ID: " + n->name); leave();   }
    void visit(Binary* bexpr) {
        enter();
        switch (bexpr->op[0]) {
            case '+': print("(add "); break;
            case '-': print("(sub "); break;
            case '*': print("(mul "); break;
            case '/': print("(div "); break;
            case ':': print("(assign "); break;
            case '<': print("(less "); break;
        }
        if (bexpr->left != nullptr)
            bexpr->left->accept(this);
        if (bexpr->right != nullptr)
            bexpr->right->accept(this);
        print(")");
        leave();
    }
    void visit(Unary* uexpr) {
        enter();
        print("Op: " + uexpr->op);
        uexpr->left->accept(this);
        leave();
    }
};

#endif