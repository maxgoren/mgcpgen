#ifndef ast_hpp
#define ast_hpp
#include "token.hpp"

struct Atom;
struct List;

struct Visitor {
    virtual void visit(Atom* ast) = 0;
    virtual void visit(List* ast) = 0;
};

struct AST {
    virtual ~AST() = default;
    virtual void accept(Visitor* v) = 0;
};

struct Atom : AST {
    Token value;
    Atom(Token& t) : value(t) { }
    void accept(Visitor* v) {
        v->visit(this);
    }
};

struct List : AST {
    vector<AST*> elements;
    List() {

    }
    void accept(Visitor* v) {
        v->visit(this);
    }
};

class SexprVisitor : Visitor {
    public:
    void visit(Atom* a) {
        cout<<a->value.getString()<<" ";
    }
    void visit(List* l) {
        cout<<"(";
        for (auto m : l->elements)
            m->accept(this);
        cout<<")";
    }
};



#endif