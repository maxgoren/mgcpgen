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
struct DefStmt;
struct LetStmt;
struct WhileStmt;
struct PrintStmt;
struct IfStmt;
struct StmtSequence;
struct ExprStmt;

struct Visitor {
    virtual void visit(Identifier* ast) = 0;
    virtual void visit(Number* ast) = 0;
    virtual void visit(Binary* ast) = 0;
    virtual void visit(Unary* ast) = 0;
    virtual void visit(ExprStmt* ast) = 0;
    virtual void visit(LetStmt* ast) = 0;
    virtual void visit(WhileStmt* ast) = 0;
    virtual void visit(PrintStmt* ast) = 0;
    virtual void visit(IfStmt* ast) = 0;
    virtual void visit(StmtSequence* ast) = 0;
    virtual void visit(DefStmt* ast) = 0;
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
    ExprStmt(AST* e) : expr(e) { }
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

struct DefStmt : AST {
    Identifier* name;
    StmtSequence* body;
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
        //ds->name->accept(this);
        //ds->body->accept(this);
        procedures[ds->name->name] = ds;
    }
    map<string, DefStmt*> procedures;    
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
};

#endif