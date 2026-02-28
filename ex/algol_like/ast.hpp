#ifndef ast_hpp
#define ast_hpp
#include <iostream>
#include <unordered_map>
using namespace std;
unordered_map<string, int> valTable;

struct AST;
struct Identifier;
struct Number;
struct Binary;
struct Unary;
//struct ExprStmt;
struct LetStmt;
struct WhileStmt;
struct PrintStmt;
struct IfStmt;
struct StmtSequence;

struct Visitor {
    virtual void visit(Identifier* ast) = 0;
    virtual void visit(Number* ast) = 0;
    virtual void visit(Binary* ast) = 0;
    virtual void visit(Unary* ast) = 0;
    //virtual void visit(ExprStmt* ast) = 0;
    virtual void visit(LetStmt* ast) = 0;
    virtual void visit(WhileStmt* ast) = 0;
    virtual void visit(PrintStmt* ast) = 0;
    virtual void visit(IfStmt* ast) = 0;
    virtual void visit(StmtSequence* ast) = 0;
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

struct StmtSequence : AST {
    StmtSequence* next;
    AST* stmt;
    StmtSequence() {
        stmt = nullptr;
        next = nullptr;
    }
    void addStmt(AST* sast) {
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
    PrintStmt() { }
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
    WhileStmt() { }
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
    void accept(Visitor* v) {
        v->visit(this);
    }
};

struct LetStmt : AST {
    AST* expr;
    void accept(Visitor* v) {
        v->visit(this);
    }
};

struct EvalVisitor : Visitor {
    stack<int> sv;
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

    }
    void visit(IfStmt* is) {
        is->testExpr->accept(this); 
        sv.top() ? is->truePath->accept(this):is->falsePath->accept(this);
    }
    void visit(StmtSequence* ss) {
        if (ss->stmt != nullptr) {
            ss->stmt->accept(this);
        }
        if (ss->next != nullptr) {
            ss->next->accept(this);
        }
    }
};
struct PrintVisitor : Visitor {
    void visit(Number* n) { cout<<"Num: "<<n->value; }
    void visit(Identifier* n) { cout<<"ID: "<<n->name;    }
    void visit(Binary* bexpr) {
        cout<<"Op: "<<bexpr->op;
        bexpr->left->accept(this);
        bexpr->right->accept(this);
    }
    void visit(Unary* uexpr) {
        cout<<"Op: "<<uexpr->op;
        uexpr->left->accept(this);
    }
    void visit(PrintStmt* ps) {
        cout<<"PrintStmt: ";
        ps->expr->accept(this);
    }
    void visit(WhileStmt* ws) {
        cout<<"While Stmnt: ";
        ws->testExpr->accept(this);
        ws->body->accept(this);
    }
    void visit(IfStmt* is) {
                cout<<"if stmt ";
        is->testExpr->accept(this);
        cout<<"{";
        is->truePath->accept(this);
        cout<<"}"<<endl;
    }
    void visit(LetStmt* ls) {
        ls->expr->accept(this);
    }
    void visit(StmtSequence* ss) {
        if (ss->stmt != nullptr) {
            ss->stmt->accept(this);
        }
        if (ss->next != nullptr) {
            ss->next->accept(this);
        }
    }
};

#endif