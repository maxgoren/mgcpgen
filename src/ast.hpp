#ifndef ast_hpp
#define ast_hpp
#include <iostream>
using namespace std;

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
        return 0;
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
        cout<<")"<<endl;
    }
    int eval() {
        int lhs = (left == nullptr) ? 0:left->eval();
        int rhs = (right == nullptr) ? 0:right->eval();
        cout<<"lhs: "<<lhs<<endl;
        cout<<"rhs: "<<rhs<<endl;
        cout<<op[0]<<"?";
        switch (op[0]) {
            case '+': return lhs + rhs;
            case '-': return lhs - rhs;
            case '/': return lhs / rhs;
            case '*': return lhs * rhs;
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
    vector<AST*> sequence;
    StmtSequence() {

    }
    void addStmt(AST* sast) {
        sequence.push_back(sast);
    }
    void print() {
        cout<<"Executing Statement Sequence: ";
        for (auto stmt : sequence) {
            stmt->print();
        }
    }
    int eval() {
        for (auto stmt : sequence) {
            stmt->eval();
        }
        return 0;
    }
};

struct PrintStmt : AST {
    AST* expr;
    PrintStmt(AST* ast) : expr(ast) { }
    void print() {
        cout<<"PrintStmt ";
        if (expr != nullptr) expr->print();
    }
    int eval() {
        int result = (expr == nullptr) ? 0:expr->eval();
        cout<<result<<endl;
        return result;
    }
};

#endif