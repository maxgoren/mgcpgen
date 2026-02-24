#ifndef ast_hpp
#define ast_hpp
#include <iostream>
using namespace std;

struct AST {
    virtual ~AST() = default;
    virtual void print() = 0;
    virtual string eval() = 0;
};

struct Atom : AST {
    string data;
    Atom(string s) : data(s) { }
    void print() {
        cout<<data<<" ";
    }
    virtual string eval() {
        return data;
    }
};


struct Cons : AST {
    AST* car;
    AST* cdr;
    Cons() { car = nullptr; cdr = nullptr; }
    void print() {
        
        if (car != nullptr) {
            cout<<"("; 
            if (cdr != nullptr) {
                cdr->print();
                cout<<". ";
            }
            car->print();
            cout<<") ";
        }
    }
    void append(AST* ast) {
        if (car == nullptr) {
            car = ast;
            return;
        } else if (cdr == nullptr) {
            cdr = new Cons();
        } 
        dynamic_cast<Cons*>(cdr)->append(ast);        
    }
    string eval() {
        if (cdr != nullptr) cdr->eval();
        return car != nullptr ? car->eval():"()";
    }
};

#endif