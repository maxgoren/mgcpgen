#pragma once
#include "../parse/ast.hpp"
#include "object.hpp"
#include <unordered_map>
#include <iostream>
#include <vector>
#include <stack>
using namespace std;

typedef unordered_map<string, Object> Environment;

struct Frame {
    Environment bindings;
    Frame* dynamicLink;
    Frame(Environment e, Frame* p) : bindings(e), dynamicLink(p) { }
    Frame() { }
};

struct Context {
    unordered_map<string, Object> globals;
    Frame* symtab;
    unordered_map<string, Object> funcTab;
    unordered_map<string, AST*> userTypes;
    stack<Object> st;
    Context() {
        symtab = new Frame(Environment(), nullptr);
    }
    void get(string name, int d) {
        auto it = symtab;
        while (it != nullptr) {
            if ((it)->bindings.find(name) != (it)->bindings.end()) {
                st.push((it)->bindings[name]);
                return;
            }
            it = it->dynamicLink;
        }
        if (globals.find(name) != globals.end()) {
            st.push(globals[name]);
            return;
        }        
        std::cout<<"Couldn't find "<<name<<endl;
    }
    void put(string name, int d, Object obj) {
        auto it = symtab;
        while (it != nullptr) {
            if ((it)->bindings.find(name) != (it)->bindings.end()) {
                (it)->bindings[name] = obj;
                return;
            }
            it = it->dynamicLink;
        }
        if (globals.find(name) != globals.end()) {
            globals[name] = obj;
            return;
        }
        if (symtab)
            symtab->bindings[name] = obj;
        else globals[name] = obj;
    }
    void openScope() {
        openScope(Environment());
    }
    void openScope(Environment e) {
        symtab = new Frame(e, symtab);
    }
    void closeScope() {
        if (symtab) {
            auto t = symtab;
            symtab = symtab->dynamicLink;
        }
    }
};

class ScopeResolution {
    private:
        vector<unordered_map<string, bool>> scopes;
        void openScope() {
            scopes.push_back(unordered_map<string,bool>());
        }
        void closeScope() {
            scopes.pop_back();
        }
        void defineId(string name) {
            if (scopes.empty())
                return;
            if (scopes.back().find(name) == scopes.back().end()) {
                scopes.back()[name] = true;
            }
        }
        void resolveName(AST* ast, string name) {
            for (int i = scopes.size()-1; i >= 0; i--) {
                if (scopes[i].find(name) != scopes[i].end()) {
                    ast->token.setScopeLevel(scopes.size() - 1 - i);
                    cout<<"Resolved "<<name<<" at scope depth "<<scopes.size()-1-i<<endl;
                    return;
                }
            }
        }
        void resolveExpr(AST* ast) {
            switch (ast->attr.expr) {
                case ID_EXPR: {
                    resolveName(ast, ast->token.getString());
                } break;
                default: break;
            }
            for (int i = 0; i < 3; i++)
                traverse(ast->children[i]);
        }
        void resolveStmt(AST* ast) {
            switch (ast->attr.stmt) {
                case BLOCK_STMT: {
                    openScope();
                    traverse(ast->children[0]);
                    closeScope();
                } break;
                case DEF_STMT: {
                    defineId(ast->children[0]->token.getString());
                    openScope();
                    for (int i = 0; i < 3; i++) 
                        traverse(ast->children[i]);
                    closeScope();
                } break;
                default: break;
            }
        }
        void traverse(AST* ast) {
            if (ast != nullptr) {
                if (ast->attr.type == EXPR_NODE) {
                    resolveExpr(ast);
                } else {
                    resolveStmt(ast);
                }
                traverse(ast->next);
            }
        }
    public:
        ScopeResolution() {

        }
        void resolveScopes(AST* ast) {
            
        }
};

class Interpreter {
    private:
        bool bail;
        Context cxt;
        bool isbuiltin(string name);
        void dobuiltin(AST* ast);
        void evalParams(AST* params, AST* args);
        void evalLambdaFunc(AST* params, AST* args, AST* body);
        void stmt(AST* ast);
        void eval(AST* ast);
        void evalBinOp(AST* ast);
        void evalUnaryOp(AST* ast);
    public:
        Interpreter() {

        }
        void exec(AST* ast);
};

bool Interpreter::isbuiltin(string name) {
    if (name == "push")  {
        return true;
    }
    if (name == "size") {
        return true;
    }
    return false;
}
void Interpreter::dobuiltin(AST* ast) {
    string fname = ast->token.getString();
    AST* args = ast->children[1];
    if (fname == "push")  {
        eval(args);
        eval(args->next);
        Object tp = cxt.st.top(); cxt.st.pop();
        cxt.st.top().listval->push_front(tp);
        cxt.st.pop();
    }
    if (fname == "size") {
        eval(args);
        int size = cxt.st.top().listval->size();
        cxt.st.pop();
        cxt.st.push(Object((double)size));
    }
}



void Interpreter::evalParams(AST* par, AST* ar) {
    unordered_map<string, Object> tmp;
    auto params = par;
    auto args = ar;
    while (params != nullptr && args != nullptr) {
        string nm = params->children[0]->token.getString();
        eval(args);
        tmp[nm] = cxt.st.top(); cxt.st.pop();
        cout<<"Assigned "<<tmp[nm].toString()<<" to "<<nm<<endl;
        params = params->next;
        args = args->next;
    }
    cxt.openScope(tmp);
}

void Interpreter::evalLambdaFunc(AST* params, AST* args, AST* body) {
    evalParams(params, args);
    exec(body);
    bail = false;
    cxt.closeScope();
}

void Interpreter::eval(AST* ast) {
    if (ast != nullptr) {
        eval(ast->next);
        if (ast->children[0] == nullptr) {
            if (ast->token.getSymbol() == TK_NUM) {
                cxt.st.push( Object(stod(ast->token.getString())));
            } else if (ast->token.getSymbol() == TK_ID) {
                cxt.get(ast->token.getString(), ast->token.scopeLevel());
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
                string fname = ast->token.getString();
                if (isbuiltin(fname)) {
                    cout<<"Executing builtin: "<<fname<<endl;
                    dobuiltin(ast);
                    return;
                }
                Object t;
                if (ast->children[0]->attr.expr == ID_EXPR) {
                    if (cxt.funcTab.find(fname) != cxt.funcTab.end())
                        t = cxt.funcTab[fname];
                    else {
                        cxt.get(fname, 0);
                        t = cxt.st.top();
                        cxt.st.pop();
                    }
                    cout<<"Executing function: "<<t.funcval->name<<endl;
                } else if (ast->children[0]->attr.expr == LAMBDA_EXPR) {
                    eval(ast->children[0]);
                    t = cxt.st.top(); cxt.st.pop();
                }
                Function* f = t.funcval;
                AST* params = f->params;
                AST* args = ast->children[1];
                cout<<"Evaluating arguments: "<<endl;
                evalLambdaFunc(params, args, f->body);
            } else if (ast->attr.expr == LAMBDA_EXPR) {
                Function* lf = new Function("&", ast->children[1], ast->children[2]);
                cxt.st.push(Object(lf));
            } else if (ast->attr.expr == UNARY_EXPR) {
                evalUnaryOp(ast);
            } else {
                evalBinOp(ast);
            }
        }
    }
}

void Interpreter::evalUnaryOp(AST* ast) {
    eval(ast->children[0]);
    Object v = cxt.st.top(); cxt.st.pop();
    v.numval = -v.numval;
    cxt.st.push(v);
}

void Interpreter::evalBinOp(AST* ast) {
    if (ast->token.getSymbol() == TK_ASSIGN) {
        eval(ast->children[1]);
        if (ast->children[0]->attr.expr == ID_EXPR) {
            cxt.put(ast->children[0]->token.getString(), ast->children[0]->token.scopeLevel(), cxt.st.top());
        } else if (ast->children[0]->attr.expr == SUBSCRIPT_EXPR) {
            eval(ast->children[0]->children[0]);
            eval(ast->children[0]->children[1]);
            Object idx = cxt.st.top(); cxt.st.pop();
            Object lval = cxt.st.top(); cxt.st.pop();
            lval.listval->at(idx.numval) = cxt.st.top(); cxt.st.pop();
            cxt.symtab->bindings[ast->children[0]->children[0]->token.getString()] = lval;
            cxt.st.push(lval);
        }
        return;
    }
    eval(ast->children[0]);
    Object lho = cxt.st.top(); cxt.st.pop();
    eval(ast->children[1]);
    Object rho = cxt.st.top(); cxt.st.pop();
    double lhs = lho.numval;
    double rhs = rho.numval;
    cout<<"Performing "<<ast->token.getString()<<" to "<<lhs<<" and "<<rhs<<endl;
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
        case TK_EQU:  cxt.st.push((bool)(lhs == rhs)); break;
        case TK_NEQ:  cxt.st.push((bool)(lhs != rhs)); break;
        case TK_LT:   cxt.st.push((bool)(lhs < rhs));  break;
        case TK_GT:   cxt.st.push((bool)(lhs > rhs));  break;
        default:
            break;
    }
}

void Interpreter::stmt(AST* ast) {
    switch (ast->attr.stmt) {
        case PRINT_STMT:
                eval(ast->children[0]);
                cout<<cxt.st.top().toString()<<endl;
            break;
        case BLOCK_STMT: {
            cxt.openScope();
            exec(ast->children[0]);
            cxt.closeScope();
        } break;
        case IF_STMT:
            eval(ast->children[0]);
            if (cxt.st.top().boolval) {
                exec(ast->children[1]);
            }
        break;
        case STRUCT_STMT: {
            cxt.userTypes[ast->children[0]->token.getString()] = ast;
        } break;
        case WHILE_STMT:
            eval(ast->children[0]);
            while (cxt.st.top().boolval) {
                cxt.st.pop();
                exec(ast->children[1]);
                eval(ast->children[0]);
            }
            cxt.st.pop();
            break;
        case LET_STMT:
            eval(ast->children[0]);
        break;
        case RETURN_STMT:
            eval(ast->children[0]);
            bail = true;
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
    if (ast != nullptr) {
        if (ast->attr.type == STMT_NODE) {
            stmt(ast);
        } else {
            eval(ast);
        }
        if (ast->next && bail == false)
            exec(ast->next);
    }
}