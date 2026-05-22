#pragma once
#include "../parse/ast.hpp"
#include "object.hpp"
#include <unordered_map>
#include <iostream>
#include <vector>
using namespace std;

class ReturnException : public std::exception {
    virtual const char* what() const throw() {
        return "ret stmt";
    }
};

struct Context {
    vector<unordered_map<string, Object>> symtab;
    unordered_map<string, Object> funcTab;
    stack<Object> st;
    Context() {
        symtab.push_back(unordered_map<string,Object>());
    }
};

class Interpreter {
    private:
        Context cxt;
        void stmt(AST* ast);
        void eval(AST* ast);
        void evalBinOp(AST* ast);
    public:
        Interpreter() {

        }
        void exec(AST* ast);
};

void Interpreter::eval(AST* ast) {
    if (ast != nullptr) {
        eval(ast->next);
        if (ast->children[0] == nullptr) {
            if (ast->token.getSymbol() == TK_NUM) {
                cxt.st.push( Object(stod(ast->token.getString())));
            } else if (ast->token.getSymbol() == TK_ID) {
                cxt.st.push(cxt.symtab.back()[ast->token.getString()]);
            } else if (ast->token.getSymbol() == TK_STRING) {
                cxt.st.push(Object(new string(ast->token.getString())));
            } else {
                cxt.st.push(Object(0.0));
            }
        } else {
            if (ast->attr.expr == LIST_EXPR) {
                AST* itr = ast->children[0];
                Object nl(new deque<Object>());
                while (itr != nullptr) {
                    eval(itr);
                    nl.listval->push_back(cxt.st.top()); cxt.st.pop();
                    itr = itr->next;
                }
                cxt.st.push(nl);
            } else if (ast->attr.expr == SUBSCRIPT_EXPR) {
                eval(ast->children[0]);
                Object lv = cxt.st.top(); cxt.st.pop();
                eval(ast->children[1]);
                Object idx = cxt.st.top(); cxt.st.pop();
                cxt.st.push(lv.listval->at(idx.numval));
            } else if (ast->attr.expr == FUNC_EXPR) { 
                unordered_map<string, Object> tmp;
                Object t = cxt.funcTab[ast->token.getString()];
                cout<<"Executing function: "<<t.funcval->name<<endl;
                Function* f = t.funcval;
                AST* params = f->params;
                AST* args = ast->children[1];
                cout<<"Evaluating arguments: "<<endl;
                while (params != nullptr && args != nullptr) {
                    string nm = params->children[0]->token.getString();
                    eval(args);
                    tmp[nm] = cxt.st.top(); cxt.st.pop();
                    cout<<"Assigned "<<tmp[nm].toString()<<" to "<<nm<<endl;
                    params = params->next;
                    args = args->next;
                }
                cxt.symtab.push_back(tmp);
                try {
                    exec(f->body);
                } catch (ReturnException re) {

                }
                cxt.symtab.pop_back();
            } else {
                evalBinOp(ast);
            }
        }
    }
}

void Interpreter::evalBinOp(AST* ast) {
    if (ast->token.getSymbol() == TK_ASSIGN) {
        eval(ast->children[1]);
        if (ast->children[0]->attr.expr == ID_EXPR) {
            cxt.symtab.back()[ast->children[0]->token.getString()] = cxt.st.top(); cxt.st.pop();
            cxt.st.push(cxt.symtab.back()[ast->children[0]->token.getString()]);
        } else if (ast->children[0]->attr.expr == SUBSCRIPT_EXPR) {
            eval(ast->children[0]->children[0]);
            eval(ast->children[0]->children[1]);
            Object idx = cxt.st.top(); cxt.st.pop();
            Object lval = cxt.st.top(); cxt.st.pop();
            lval.listval->at(idx.numval) = cxt.st.top(); cxt.st.pop();
            cxt.symtab.back()[ast->children[0]->children[0]->token.getString()] = lval;
            cxt.st.push(lval);
        }
    }
    eval(ast->children[0]);
    Object lho = cxt.st.top(); cxt.st.pop();
    eval(ast->children[1]);
    Object rho = cxt.st.top(); cxt.st.pop();
    double lhs = lho.numval;
    double rhs = rho.numval;
    cout<<"Performing "<<ast->token.getString()<<" to "<<lho.toString()<<" and "<<rho.toString()<<endl;
    switch (ast->token.getSymbol()) {
        case TK_MINUS: {
            if (ast->children[1] == nullptr) {
                cxt.st.push(-lhs);
            } else {
                cxt.st.push(lhs-rhs);
            }
        } break;
        case TK_PLUS: cxt.st.push(lhs+rhs); break;
        case TK_MUL:  cxt.st.push(lhs*rhs); break;
        case TK_DIV:  cxt.st.push(lhs/rhs); break;
        case TK_EQU:  cxt.st.push(lhs == rhs); break;
        case TK_NEQ:  cxt.st.push(lhs != rhs); break;
        case TK_LT:   cxt.st.push(lhs < rhs);  break;
        case TK_GT:   cxt.st.push(lhs > rhs);  break;
        default:
            break;
    }
}

void Interpreter::stmt(AST* ast) {
    switch (ast->attr.stmt) {
            case PRINT_STMT:
                exec(ast->children[0]);
                cout<<cxt.st.top().toString()<<endl;
            break;
        case BLOCK_STMT:
            exec(ast->children[0]);
        break;
        case IF_STMT:
            eval(ast->children[0]);
            if (cxt.st.top().boolval) {
                exec(ast->children[1]);
            }
        break;
        case LET_STMT:
            eval(ast->children[0]);
        break;
        case RETURN_STMT:
            eval(ast->children[0]);
            throw ReturnException();
        break;
        case DEF_STMT: {
            string name = ast->children[0]->token.getString();
            cxt.funcTab[name] = new Function(name, ast->children[1], ast->children[2]);
        } break;
        default:
            eval(ast);
            break;
    }
}

void Interpreter::exec(AST* ast) {
    Object val = 0.0;
    if (ast != nullptr) {
        if (ast->attr.type == STMT_NODE) {
            stmt(ast);
        } else {
            eval(ast);
        }
        if (ast->next)
            exec(ast->next);
    }
}