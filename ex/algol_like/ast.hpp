#ifndef ast_hpp
#define ast_hpp
#include <iostream>
using namespace std;

struct AST;
struct Identifier;
struct Number;
struct Binary;
struct Unary;
struct FuncExpr;
struct DefStmt;
struct LetStmt;
struct WhileStmt;
struct PrintStmt;
struct IfStmt;
struct StmtSequence;
struct ExprStmt;
struct ArgsList;
struct ExprList;

struct Visitor {
    virtual void visit(Identifier* ast) = 0;
    virtual void visit(Number* ast) = 0;
    virtual void visit(Binary* ast) = 0;
    virtual void visit(Unary* ast) = 0;
    virtual void visit(FuncExpr* ast) = 0;
    virtual void visit(ExprStmt* ast) = 0;
    virtual void visit(LetStmt* ast) = 0;
    virtual void visit(WhileStmt* ast) = 0;
    virtual void visit(PrintStmt* ast) = 0;
    virtual void visit(IfStmt* ast) = 0;
    virtual void visit(StmtSequence* ast) = 0;
    virtual void visit(DefStmt* ast) = 0;
    virtual void visit(ArgsList* ast) = 0;
    virtual void visit(ExprList* ast) = 0;
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

struct FuncExpr : AST {
    Identifier* name;
    ExprList* args;
    FuncExpr() { 
        name = nullptr;
        args = nullptr;
    }
    void accept(Visitor* v) {
        v->visit(this);
    }
};

bool isExpression(AST* ast) {
    if (dynamic_cast<Binary*>(ast) != nullptr)
        return true;
    if (dynamic_cast<Unary*>(ast) != nullptr)
        return true;
    if (dynamic_cast<Number*>(ast) != nullptr)
        return true;
    if (dynamic_cast<Identifier*>(ast) != nullptr)
        return true;
    return false;
}

struct ExprStmt : AST {
    AST* expr;
    ExprStmt(AST* e = nullptr) : expr(e) { }
    void accept(Visitor* v) {
        v->visit(this);
    }
};

struct StmtSequence : AST {
    StmtSequence* next;
    AST* stmt;
    StmtSequence() {
        stmt = nullptr;
        next = nullptr;
    }
    StmtSequence(AST* s) {
        stmt = s;
        next = nullptr;
    }
    StmtSequence(AST* s1, AST* s2) {
        stmt = s1;
        addStmt(s2);
    }
    void addStmt(AST* sast) {
        if (sast == nullptr)
            return;
        if (stmt == nullptr) {
            stmt = sast;
        } else {
            if (next == nullptr) {
                next = new StmtSequence();
            }
            next->addStmt(sast);
        }
    }
    void accept(Visitor* v) {
        v->visit(this);
    }
};

bool isStmtSequence(AST* ast) {
    return dynamic_cast<StmtSequence*>(ast) != nullptr;
}

struct PrintStmt : AST {
    AST* expr;
    PrintStmt() { expr = nullptr; }
    PrintStmt(AST* ast) : expr(ast) { }
    void accept(Visitor* v) {
        v->visit(this);
    }
};

bool isPrintStmt(AST* ast) {
    return dynamic_cast<PrintStmt*>(ast) != nullptr;
}

struct WhileStmt : AST {
    AST* testExpr;
    AST* body;
    WhileStmt() { testExpr = nullptr; body = nullptr; }
    void accept(Visitor* v) {
        v->visit(this);
    }
};

bool isWhileStmt(AST* ast) {
    return dynamic_cast<WhileStmt*>(ast) != nullptr;
}

struct IfStmt : AST {
    AST* testExpr;
    AST* truePath;
    AST* falsePath;
    IfStmt() {
        testExpr = nullptr;
        truePath = nullptr;
        falsePath = nullptr;
    }
    void accept(Visitor* v) {
        v->visit(this);
    }
};

struct LetStmt : AST {
    AST* expr;
    LetStmt() {
        expr = nullptr;
    }
    void accept(Visitor* v) {
        v->visit(this);
    }
};

struct DefStmt : AST {
    Identifier* name;
    ArgsList* args;
    StmtSequence* body;
    DefStmt() {
        name = nullptr;
        args = nullptr;
        body = nullptr;
    }
    void accept(Visitor* v) {
        v->visit(this);
    }
};

struct ArgsList : AST {
    vector<LetStmt*> arguments;
    void accept(Visitor* v) {
        v->visit(this);
    }
};

struct ExprList : AST {
    vector<AST*> arguments;
    void accept(Visitor* v) {
        v->visit(this);
    }
};

#endif