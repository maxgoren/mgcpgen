#pragma once
#include "ast.hpp"
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

void preorder(AST* ast, int d) {
    if (ast != nullptr) {
        for (int i = 0; i < d; i++) cout<<" ";
        cout<<nodeTypeStr[ast->type]<<" ";
        cout<<ast->token.getString()<<endl;
        for (AST* n : ast->children) {
            preorder(n, d+1);
        }
        preorder(ast->next, d);
    }
}

void exec(AST* ast);
void evalBinOp(AST* ast);

unordered_map<string, Object> symtab;
unordered_map<string, Object> funcTab;
stack<Object> st;

void eval(AST* ast) {
    if (ast != nullptr) {
        eval(ast->next);
        if (ast->children[0] == nullptr) {
            if (ast->token.getSymbol() == TK_NUM) {
                st.push( Object(stod(ast->token.getString())));
            } else if (ast->token.getSymbol() == TK_ID) {
                st.push(symtab[ast->token.getString()]);
            } else if (ast->token.getSymbol() == TK_STRING) {
                st.push(Object(new string(ast->token.getString())));
            } else {
                st.push(Object(0.0));
            }
        } else {
            if (ast->type == FUNC_EXPR) {
                unordered_map<string, Object> tmp;
                Object t = funcTab[ast->token.getString()];
                cout<<"Executing function: "<<t.funcval->name<<endl;
                Function* f = t.funcval;
                AST* params = f->params;
                AST* args = ast->children[1];
                cout<<"Evaluating arguments: "<<endl;
                while (params != nullptr && args != nullptr) {
                    string nm = params->children[0]->token.getString();
                    eval(args);
                    tmp[nm] = st.top(); st.pop();
                    cout<<"Assigned "<<st.top().toString()<<" to "<<nm<<endl;
                    params = params->next;
                    args = args->next;
                }
                auto prev = symtab;
                symtab = tmp;
                try {
                    exec(f->body);
                } catch (ReturnException re) {

                }
                symtab = prev;
            } else {
                evalBinOp(ast);
            }
        }
    }
}

void evalBinOp(AST* ast) {
    if (ast->token.getSymbol() == TK_ASSIGN) {
        eval(ast->children[1]);
        symtab[ast->children[0]->token.getString()] = st.top(); st.pop();
        st.push(symtab[ast->children[0]->token.getString()]);
    }
    eval(ast->children[0]);
    Object lho =st.top(); st.pop();
    eval(ast->children[1]);
    Object rho = st.top(); st.pop();
    double lhs = lho.numval;
    double rhs = rho.numval;
    cout<<"Performing "<<ast->token.getString()<<" to "<<lhs<<" and "<<rhs<<endl;
    switch (ast->token.getSymbol()) {
        case TK_MINUS: {
            if (ast->children[1] == nullptr) {
                st.push(-lhs);
            } else {
                st.push(lhs-rhs);
            }
        } break;
        case TK_PLUS: st.push(lhs+rhs); break;
        case TK_MUL:  st.push(lhs*rhs); break;
        case TK_DIV:  st.push(lhs/rhs); break;
        case TK_EQU:  st.push(lhs == rhs); break;
        case TK_NEQ:  st.push(lhs != rhs); break;
        case TK_LT:   st.push(lhs < rhs);  break;
        case TK_GT:   st.push(lhs > rhs);  break;
        default:
            break;
    }
}

void exec(AST* ast) {
    Object val = 0.0;
    if (ast != nullptr) {
        switch (ast->type) {
            case PRINT_STMT:
                exec(ast->children[0]);
                cout<<st.top().toString()<<endl;
            break;
            case BLOCK_STMT:
                exec(ast->children[0]);
            break;
            case IF_STMT:
                eval(ast->children[0]);
                if (st.top().boolval) {
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
                funcTab[name] = new Function(name, ast->children[1], ast->children[2]);
            } break;
            default:
                eval(ast);
                break;
        }
        if (ast->next)
            exec(ast->next);
    }
}