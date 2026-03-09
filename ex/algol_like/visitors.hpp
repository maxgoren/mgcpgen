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
    unordered_map<string, int> valTable;
    stack<unordered_map<string,int>> envStack;
    map<string, DefStmt*> procedures;
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
    void visit(PrintStmt* ps) {
        ps->expr->accept(this);
        cout<<sv.top()<<endl;
        sv.pop();
    }
    void visit(WhileStmt* ws) {
        ws->testExpr->accept(this);
        while (sv.top() == 1) {
            ws->body->accept(this);
            ws->testExpr->accept(this);
        } 
        sv.pop();
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
        string id = (ds->name == nullptr) ? "":ds->name->name;
        if (id != "") {
            cout<<"Storing procedure as "<<id<<endl;
            procedures[id] = ds;
        } else {
            cout<<"Cant store a procedure without a name, chief."<<endl;
        }
    }
    void visit(FuncExpr* expr) {
        string name = "";
        if (expr->name != nullptr) {
            name = expr->name->name;
        } else {
            cout<<"Missing a name, bailing out."<<endl;
            return;
        }
        cout<<"Getting procedure: "<<name<<endl;
        DefStmt* ds = procedures[name];
        if (ds != nullptr) {
            envStack.push(valTable);
            valTable = unordered_map<string,int>();
            auto names = ds->args->arguments.begin();
            auto values = expr->args->arguments.begin();
            while (names != ds->args->arguments.end() && values != expr->args->arguments.end()) {
                string name = dynamic_cast<Identifier*>((*names)->expr)->name;
                (*values)->accept(this);
                valTable[name] = sv.top(); sv.pop();
                names++;
                values++;
            }
            ds->body->accept(this);
            valTable = envStack.top(); envStack.pop();
        } else {
            cout<<"Bumbaclot pon "<<expr->name->name<<endl;
        }
    }
    void eval(AST* ast) {
        if (ast != nullptr && dynamic_cast<StmtSequence*>(ast) != nullptr) {
            visit(dynamic_cast<StmtSequence*>(ast));
        } else if (ast != nullptr && dynamic_cast<ExprStmt*>(ast) != nullptr) {
            visit(dynamic_cast<ExprStmt*>(ast));
        } else if (ast != nullptr && dynamic_cast<WhileStmt*>(ast) != nullptr) {
            visit(dynamic_cast<WhileStmt*>(ast));
        } else if (ast != nullptr && dynamic_cast<LetStmt*>(ast) != nullptr) {
            visit(dynamic_cast<LetStmt*>(ast));
        } else if (ast != nullptr && dynamic_cast<PrintStmt*>(ast) != nullptr) {
            visit(dynamic_cast<PrintStmt*>(ast));
        } else if (dynamic_cast<DefStmt*>(ast) != nullptr) {
            visit(dynamic_cast<DefStmt*>(ast));
        } 
    }
    void visit(ArgsList* ast) {
        for (auto t : ast->arguments) {
            t->accept(this);
        }
    }
    void visit(ExprList* ast) {
        for (auto t : ast->arguments) {
            t->accept(this);
        }
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
        if (ast == nullptr) 
            return;
        if (dynamic_cast<StmtSequence*>(ast) != nullptr) {
            visit(dynamic_cast<StmtSequence*>(ast));
        } else if (ast != nullptr && dynamic_cast<ExprStmt*>(ast) != nullptr) {
            visit(dynamic_cast<ExprStmt*>(ast));
        } else if (ast != nullptr && dynamic_cast<WhileStmt*>(ast) != nullptr) {
            visit(dynamic_cast<WhileStmt*>(ast));
        } else if (ast != nullptr && dynamic_cast<LetStmt*>(ast) != nullptr) {
            visit(dynamic_cast<LetStmt*>(ast));
        } else if (ast != nullptr && dynamic_cast<PrintStmt*>(ast) != nullptr) {
            visit(dynamic_cast<PrintStmt*>(ast));
        } else if (ast != nullptr && dynamic_cast<DefStmt*>(ast) != nullptr) {
            visit(dynamic_cast<DefStmt*>(ast));
        } else if (ast != nullptr && dynamic_cast<IfStmt*>(ast) != nullptr) {
            visit(dynamic_cast<IfStmt*>(ast));
        }
    }
    void visit(Number* n) { enter(); print("Num: " + n->value); leave(); }
    void visit(Identifier* n) { enter(); print("ID: " + n->name); leave();   }
    void visit(ExprStmt* es) { es->expr->accept(this); }
    void visit(Binary* bexpr) {
        enter();
        print("(Op: " + bexpr->op);
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
    void visit(PrintStmt* ps) {
        enter();
        print("PrintStmt: ");
        ps->expr->accept(this);
        leave();
    }
    void visit(WhileStmt* ws) {
        enter();
        print("While Stmnt: ");
        if (ws->testExpr != nullptr) {
            print("  Predicate: ");
            ws->testExpr->accept(this);
        } else {
            cout<<"Error: Missing predicate."<<endl;
        }
        if (ws->body != nullptr) {
            print("  Body: ");
            ws->body->accept(this);
        }
        leave();
    }
    void visit(IfStmt* is) {
        enter();
        print("if stmt ");
        print("  Predicate: ");
        is->testExpr->accept(this);
        print("  True path: ");
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
        print("Procedure Def, name: ");
        if (ds->name != nullptr) {
            ds->name->accept(this);
        }
        if (ds->args != nullptr) {
            print("Arguments: ");
            ds->args->accept(this);
        }
        if (ds->body != nullptr) {
            print("Body: ");
            ds->body->accept(this);
        }
        leave();
    }
    void visit(FuncExpr* fe) {
        enter();
        print("Function call");
        fe->name->accept(this);
        if (fe->args != nullptr) {
            print("Arguments");
            fe->args->accept(this);
        }
        leave();
    }
    void visit(ArgsList* ast) {
        for (auto t : ast->arguments) {
            t->accept(this);
        }
    }
    void visit(ExprList* ast) {
        for (auto t : ast->arguments) {
            t->accept(this);
        }
    }
};

#endif