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
    void print() { cout<<"Num: "<<value<<endl; }
    int eval()  { return stoi(value);}
};

struct Identifier : AST {
    std::string name;
    Identifier(const std::string& n) : name(n) {}
    void print() {
        cout<<"ID: "<<name<<endl;
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
        switch (op[0]) {
            case '+': return lhs + rhs;
            case '-': return lhs - rhs;
            case '/': return lhs / rhs;
            case '*': return lhs * rhs;
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

AST* attachLeftmost(AST* node, AST* left) {
    Binary* b = dynamic_cast<Binary*>(node);
    while (b->right != nullptr) {
        if (b->right == nullptr)
            break;
        b = dynamic_cast<Binary*>(b->right);
    }

    b->right = left;
    return b;
}

void execute(int id, std::stack<AST*>& semStack, std::stack<Symbol>& opStack) {
    cout<<"\t \t EXECUTING: "<<id<<endl;
    switch(id) {
    case 7: {
        cout<<"print-stmtT -> expr"<<endl;
        AST* left       = semStack.top(); semStack.pop();
        semStack.push(new PrintStmt(left));
        break;
    }
    // exp → term expT
    case 9: {
        cout<<"exp → term expT"<<endl;
        AST* rightChain = semStack.top(); semStack.pop();
        AST* left       = semStack.top(); semStack.pop();

        if (rightChain == nullptr)
            semStack.push(left);
        else {
            rightChain = attachLeftmost(rightChain, left);
            semStack.push(rightChain);
        }
        break;
    }

    // expT → addop term expT
    case 10:
    case 11: {
        cout<<"expT → addop term expT"<<endl;
        AST* chain = semStack.top(); semStack.pop();
        AST* term  = semStack.top(); semStack.pop();
        std::string op = opStack.top(); opStack.pop();

        AST* node = new Binary(op, term, chain);
        node->print();
        semStack.push(node);
        break;
    }

    // expT → ε
    case 12:
        semStack.push(nullptr);
        break;

    // term → factor termT
    case 13: {
        AST* rightChain = semStack.top(); semStack.pop();
        AST* left       = semStack.top(); semStack.pop();

        if (rightChain == nullptr){
            semStack.push(left);
            semStack.top()->print();
        } else {
            rightChain = attachLeftmost(rightChain, left);
            semStack.push(rightChain);
            semStack.top()->print();
        }
        break;
    }

    // termT → mulop factor termT
    case 14:
    case 15: {
        cout<<"termT → mulop factor termT"<<endl;
        AST* chain  = semStack.top(); semStack.pop();
        AST* factor = semStack.top(); semStack.pop();
        std::string op = opStack.top(); opStack.pop();

        AST* node = new Binary(op, factor, chain);
        semStack.push(node);
        semStack.top()->print();
        break;
    }

    // termT → ε
    case 16:
        semStack.push(nullptr);
        break;

    // factor → TK_ID
    case 17:
        // already pushed by terminal
        break;

    // factor → TK_NUM
    case 18:
        // already pushed by terminal
        break;

    // factor → ( exp )
    case 19:
        // exp already on stack
        break;

    default:
        break;
    }
}

#endif