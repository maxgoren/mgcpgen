#ifndef ast_hpp
#define ast_hpp
#include <iostream>
using namespace std;

struct AST;
struct Identifier;
struct Number;
struct Binary;
struct Unary;

struct Visitor {
    virtual void visit(Identifier* ast) = 0;
    virtual void visit(Number* ast) = 0;
    virtual void visit(Binary* ast) = 0;
    virtual void visit(Unary* ast) = 0;
};

struct AST {
    virtual ~AST() = default;
    virtual void accept(Visitor* visitor) = 0;
};

struct Number : AST {
    std::string value;
    Number(const std::string& v) : value(v) {}
    void accept(Visitor* visitor) {
        visitor->visit(this);
    }
};

struct Identifier : AST {
    std::string name;
    Identifier(const std::string& n) : name(n) {}
    void accept(Visitor* visitor) {
        visitor->visit(this);
    }
};

struct Binary : AST {
    std::string op;
    AST* left;
    AST* right;

    Binary(const std::string& o, AST* l, AST* r)
        : op(o), left(l), right(r) {}
    void accept(Visitor* v) {
        v->visit(this);
    }
};

struct Unary : AST {
    std::string op;
    AST* left;

    Unary(const std::string& o, AST* l)
        : op(o), left(l) {}
    void accept(Visitor* v) {
        v->visit(this);
    }
};


#endif