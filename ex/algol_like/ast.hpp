#ifndef ast_hpp
#define ast_hpp
#include <iostream>
#include <unordered_map>
using namespace std;
unordered_map<string, int> valTable;
struct AST {
    virtual ~AST() = default;
    virtual void print() = 0;
    virtual int eval() = 0;
};

struct Number : AST {
    std::string value;
    Number(const std::string& v) : value(v) {}
    void print() { cout<<"Num: "<<value; }
    int eval()  { return stoi(value);}
};

struct Identifier : AST {
    std::string name;
    Identifier(const std::string& n) : name(n) {}
    void print() {
        cout<<"ID: "<<name;
    }
    int eval() {
        cout<<"Evaluating: "<<name<<endl;
        return valTable[name];
    }
};

struct Binary : AST {
    std::string op;
    AST* left;
    AST* right;

    Binary(const std::string& o, AST* l, AST* r)
        : op(o), left(l), right(r) {}
    void print() {
        cout<<"(Op: "<<op<<" ";
        if (left)left->print();
        cout<<" ";
        if (right) right->print();
        cout<<")";
    }
    int eval() {
        int lhs = (left == nullptr) ? 0:left->eval();
        int rhs = (right == nullptr) ? 0:right->eval();
        cout<<"lhs: "<<lhs<<endl;
        cout<<"rhs: "<<rhs<<endl;
        cout<<"opr: "<<op[0]<<endl;
        switch (op[0]) {
            case '+': return lhs + rhs;
            case '-': return lhs - rhs;
            case '/': return lhs / rhs;
            case '*': return lhs * rhs;
            case '<': return lhs < rhs;
            case '>': return lhs > rhs;
            case ':': if (op == ":=") {
                valTable[((Identifier*)left)->name] = rhs;
                return rhs;
            } break;
        }
        return 0;
    }
};

struct Unary : AST {
    std::string op;
    AST* left;

    Unary(const std::string& o, AST* l)
        : op(o), left(l) {}
    void print() {
        cout<<"(Op: "<<op<<" ";
        if (left)left->print();
        cout<<")"<<endl;
    }
    int eval() {
        int lhs = (left == nullptr) ? 0:left->eval();
        return lhs == 0 ? 0:(-lhs);
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
    void print() {
        if (stmt != nullptr) {
            stmt->print();
        }
        if (next != nullptr) {
            next->print();
        }
    }
    int eval() {
        stmt->eval();
        return next == nullptr ? 0:next->eval();
    }
};

bool isStmtSequence(AST* ast) {
    return dynamic_cast<StmtSequence*>(ast) != nullptr;
}

struct PrintStmt : AST {
    AST* expr;
    PrintStmt() { }
    PrintStmt(AST* ast) : expr(ast) { }
    void print() {
        cout<<"PrintStmt ";
        if (expr != nullptr) expr->print();
        cout<<endl;
    }
    int eval() {
        int result = (expr == nullptr) ? 0:expr->eval();
        cout<<result<<endl;
        return result;
    }
};

bool isPrintStmt(AST* ast) {
    return dynamic_cast<PrintStmt*>(ast) != nullptr;
}

struct WhileStmt : AST {
    AST* testExpr;
    AST* body;
    WhileStmt() { }
    void print() {
        cout<<"\nWhileStmt ";
        if (testExpr) {
            cout<<"\t";
            testExpr->print();
        }
        if (body) {
            cout<<"\n\t";
            body->print();
        }
    }
    int eval() {
        while (testExpr->eval()) {
            body->eval();
        }
        return 0;
    }
};

bool isWhileStmt(AST* ast) {
    return dynamic_cast<WhileStmt*>(ast) != nullptr;
}

struct IfStmt : AST {
    AST* testExpr;
    AST* truePath;
    AST* falsePath;
    int eval() {
        return (testExpr->eval()) ? truePath->eval():falsePath->eval();
    }
};

#endif