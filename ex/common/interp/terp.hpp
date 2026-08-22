#pragma once
#include "../parse/ast.hpp"
#include "object.hpp"
#include <unordered_map>
#include <iostream>
#include <vector>
#include <stack>
#include <cmath>
using namespace std;

typedef unordered_map<string, Object> Environment;

struct Frame {
    Environment bindings;
    Frame* dynamicLink;
    Frame* staticLink;
    Frame(Environment e, Frame* p, Frame* s) : bindings(e), dynamicLink(p), staticLink(s) { }
    Frame() { }
};

struct Context {
    unordered_map<string, Object> globals;
    Frame* symtab;
    unordered_map<string, Object> funcTab;
    unordered_map<string, Object> userTypes;
    stack<Object> st;
    Context() {
        symtab = new Frame(Environment(), nullptr, nullptr);
    }
    void get(string name, int d) {
        auto it = symtab;
        while (it != nullptr && d > 0) {
            it = it->staticLink;
            --d;
        }
        if (it->bindings.find(name) != it->bindings.end()) {
            st.push(it->bindings[name]);
            return;
        }
        if (globals.find(name) != globals.end()) {
            st.push(globals[name]);
            return;
        }        
        std::cout<<"Couldn't find "<<name<<endl;
    }
    void put(string name, int d, Object obj) {
        auto it = symtab;
        while (it != nullptr && d > 0) {
            it = it->staticLink;
            --d;
        }
        it->bindings[name] = obj;
    }
    void openScope() {
        openScope(Environment(), symtab);
    }
    void openScope(Environment e, Frame* defining) {
        symtab = new Frame(e, symtab, defining);
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
            cout<<"Open scope"<<endl;
            scopes.push_back(unordered_map<string,bool>());
        }
        void closeScope() {
            cout<<"Close scope"<<endl;
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
                case RECORD_DEF_STMT: {
                    defineId(ast->children[0]->token.getString());
                    openScope();
                    for (int i = 0; i < 3; i++) 
                        traverse(ast->children[i]);
                    closeScope();
                } break;
                case LET_STMT: {
                    auto x = ast;
                    while (x && !(x->attr.type == EXPR_NODE && x->attr.expr == ID_EXPR)) { x = x->children[0]; }
                    if (x) {
                        defineId(x->token.getString());
                    }
                } break;
                default: 
                    for (int i = 0; i < 3; i++)
                        traverse(ast->children[i]);
                break;
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
            openScope();
            traverse(ast);
            closeScope();
        }
};

class Interpreter {
    private:
        bool bail;
        Context cxt;
        bool isoperator(TKSymbol symbol);
        bool isbuiltin(string name);
        void dobuiltin(AST* ast);
        void evalParams(AST* params, AST* args, Frame* c);
        void evalLambdaFunc(AST* params, AST* args, AST* body, Frame* c);
        void evalBinOp(AST* ast);
        void evalUnaryOp(AST* ast);
        void stmt(AST* ast);
        void expr(AST* ast);
    public:
        Interpreter() {

        }
        void exec(AST* ast);
};

bool Interpreter::isoperator(TKSymbol symbol) {
    switch (symbol) {
        case TK_ASSIGN:
        case TK_MINUS:
        case TK_PLUS: 
        case TK_MUL:  
        case TK_DIV:  
        case TK_MOD:  
        case TK_EQU:  
        case TK_NEQ:  
        case TK_LT:   
        case TK_GT:   
        case TK_LTE:  
        case TK_GTE:  return true;
        default:
            break;
    }
    return false;
}

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
        expr(args);
        expr(args->next);
        Object tp = cxt.st.top(); cxt.st.pop();
        cxt.st.top().listval->push_front(tp);
        cxt.st.pop();
    }
    if (fname == "size") {
        expr(args);
        int size = cxt.st.top().listval->size();
        cxt.st.pop();
        cxt.st.push(Object((double)size));
    }
}



void Interpreter::evalParams(AST* par, AST* ar, Frame* closure) {
    unordered_map<string, Object> tmp;
    auto params = par;
    auto args = ar;
    while (params != nullptr && args != nullptr) {
        string nm = params->children[0]->token.getString();
        expr(args);
        tmp[nm] = cxt.st.top(); cxt.st.pop();
        //cout<<"Assigned "<<tmp[nm].toString()<<" to "<<nm<<endl;
        params = params->next;
        args = args->next;
    }
    cxt.openScope(tmp, closure);
}

void Interpreter::evalLambdaFunc(AST* params, AST* args, AST* body, Frame* closure) {
    evalParams(params, args, closure);
    exec(body);
    bail = false;
    cxt.closeScope();
}

void Interpreter::expr(AST* ast) {
    if (ast != nullptr) {
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
                    expr(itr);
                    nl.listval->push_back(cxt.st.top()); cxt.st.pop();
                    itr = itr->next;
                }
                cxt.st.push(nl);
            } else if (ast->attr.expr == SUBSCRIPT_EXPR) {
                expr(ast->children[0]);
                Object lv = cxt.st.top(); cxt.st.pop();
                expr(ast->children[1]);
                Object idx = cxt.st.top(); cxt.st.pop();
                cxt.st.push(lv.listval->at(idx.numval));
            } else if (ast->attr.expr == FIELD_EXPR) {
                expr(ast->children[0]);
                Object rec = cxt.st.top(); cxt.st.pop();
                string fieldname = ast->children[1]->token.getString();
                if (rec.type == RECORD) {
                    cxt.st.push(rec.recordval->fields[fieldname]);
                }
            } else if (ast->attr.expr == FUNC_EXPR) { 
                string fname = ast->token.getString();
                if (isbuiltin(fname)) {
                //    cout<<"[f()] Executing builtin: "<<fname<<endl;
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
                //    cout<<"[(f x)] Executing function: "<<t.funcval->name<<endl;
                } else if (ast->children[0]->attr.expr == LAMBDA_EXPR) {
                    expr(ast->children[0]);
                    t = cxt.st.top(); cxt.st.pop();
                }
                Function* f = t.funcval;
                AST* params = f->params;
                AST* args = ast->children[1];
              //  cout<<"Evaluating arguments: "<<endl;
                evalLambdaFunc(params, args, f->body, f->closure);
            } else if (ast->attr.expr == LAMBDA_EXPR) {
                Function* lf = new Function("&", ast->children[1], ast->children[2], cxt.symtab);
                cxt.st.push(Object(lf));
            } else if (ast->attr.expr == UNARY_EXPR) {
                evalUnaryOp(ast);
            } else if (isoperator(ast->token.getSymbol())) {
                evalBinOp(ast);
            } else if (ast->attr.expr == ARRAY_CON_EXPR) {
                cxt.st.push(Object(new deque<Object>(100)));
            } else if (ast->attr.expr == BLESS_EXPR) {
                if (cxt.userTypes.count(ast->children[0]->token.getString())) {
                    Record* nr = new Record(ast->children[0]->token.getString(), true);
                    for (auto it : cxt.userTypes[nr->typeName].recordval->fields) {
                        nr->fields[it.first] = Object();
                    }
                    cxt.st.push(Object(nr));
                }
            } else {
                cout<<"[Err?] ";
                expr(ast->children[0]);
                cout<<endl;
            }
        }
    }
}

void Interpreter::evalUnaryOp(AST* ast) {
    expr(ast->children[0]);
    Object v = cxt.st.top(); cxt.st.pop();
    switch (ast->token.getSymbol()) {
        case TK_MINUS: 
            v.numval = -v.numval;
            break;
        case TK_NOT:
            v.boolval = !v.boolval;
            break;
        default:
            break;
    }
    cxt.st.push(v);
}

void Interpreter::evalBinOp(AST* ast) {
    if (ast->token.getSymbol() == TK_ASSIGN) {
        expr(ast->children[1]);
        if (ast->children[0]->attr.expr == ID_EXPR) {
            cxt.put(ast->children[0]->token.getString(), ast->children[0]->token.scopeLevel(), cxt.st.top());
        } else if (ast->children[0]->attr.expr == SUBSCRIPT_EXPR) {
            expr(ast->children[0]->children[0]);
            expr(ast->children[0]->children[1]);
            Object idx = cxt.st.top(); cxt.st.pop();
            Object lval = cxt.st.top(); cxt.st.pop();
            lval.listval->at(idx.numval) = cxt.st.top(); cxt.st.pop();
            cxt.symtab->bindings[ast->children[0]->children[0]->token.getString()] = lval;
            cxt.st.push(lval);
        } else if (ast->children[0]->attr.expr == FIELD_EXPR) {
                expr(ast->children[0]->children[0]);
                Object rec = cxt.st.top(); cxt.st.pop();
                string fieldname = ast->children[0]->children[1]->token.getString();
                if (rec.type == RECORD) {
                    rec.recordval->fields[fieldname] = cxt.st.top(); cxt.st.pop();
                    cxt.symtab->bindings[ast->children[0]->children[0]->token.getString()] = rec;
                }
        }
        return;
    }
    if (isoperator(ast->token.getSymbol())) {
        expr(ast->children[0]);
        Object lho = cxt.st.top(); cxt.st.pop();
        expr(ast->children[1]);
        Object rho = cxt.st.top(); cxt.st.pop();
        double lhs = lho.numval;
        double rhs = rho.numval;
        cout<<"["<<ast->token.getString() <<"] Performing "<<ast->token.getString()<<" op on "<<lhs<<" and "<<rhs<<endl;
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
            case TK_MOD:  cxt.st.push(fmod(lhs, rhs)); break;
            case TK_EQU:  cxt.st.push((bool)(lhs == rhs)); break;
            case TK_NEQ:  cxt.st.push((bool)(lhs != rhs)); break;
            case TK_LT:   cxt.st.push((bool)(lhs < rhs));  break;
            case TK_GT:   cxt.st.push((bool)(lhs > rhs));  break;
            case TK_LTE:   cxt.st.push((bool)(lhs <= rhs));  break;
            case TK_GTE:   cxt.st.push((bool)(lhs >= rhs));  break;
            default:
                break;
        }
    }
}

void Interpreter::stmt(AST* ast) {
    switch (ast->attr.stmt) {
        case PROGRAM_STMT: {
            cout<<"Executing: "<<ast->children[0]->token.getString()<<endl;
        } break;
        case PRINT_STMT:
                expr(ast->children[0]);
                cout<<cxt.st.top().toString()<<endl;
            break;
        case BLOCK_STMT: {
            cxt.openScope();
            exec(ast->children[0]);
            cxt.closeScope();
        } break;
        case RECORD_DEF_STMT: {
            Record* nr = new Record(ast->children[0]->token.getString(), false);
            for (auto it = ast->children[1]; it != nullptr; it = it->next) {
                string name = it->children[0]->token.getString(); 
                nr->fields[name] = Object();
                cout<<"Adding field: "<<name<<endl;
            }
            cxt.userTypes[nr->typeName] = nr; 
            cout<<"Registered new user defined type '"<<nr->typeName<<"'"<<endl;
        } break;
        case STMT_LIST: {
            exec(ast->children[0]);
        } break;
        case IF_STMT:
            expr(ast->children[0]);
            if (cxt.st.top().boolval) {
                exec(ast->children[1]);
            } else {
                exec(ast->children[2]);
            }
        break;
        case STRUCT_STMT: {
            cxt.userTypes[ast->children[0]->token.getString()] = ast;
        } break;
        case WHILE_STMT:
            expr(ast->children[0]);
            while (cxt.st.top().boolval) {
                cxt.st.pop();
                exec(ast->children[1]);
                expr(ast->children[0]);
            }
            cxt.st.pop();
            break;
        case LET_STMT:
            cxt.symtab->bindings.insert({ast->children[0]->token.getString(), 0.0});
            expr(ast->children[0]);
            if (ast->children[1] != nullptr && ast->children[1]->attr.expr != TYPE_EXPR) {
                expr(ast->children[1]);
                cxt.symtab->bindings[ast->children[0]->token.getString()] = cxt.st.top(); cxt.st.pop();
            }
            break;
        case RETURN_STMT:
            expr(ast->children[0]);
            bail = true;
        break;
        case DEF_STMT: {
            string name = ast->children[0]->token.getString();
            cxt.funcTab[name] = new Function(name, ast->children[1], ast->children[2], cxt.symtab);
        } break;
        default:
            expr(ast);
            break;
    }
}

void Interpreter::exec(AST* ast) {
    if (ast != nullptr) {
        if (ast->attr.type == STMT_NODE) {
            stmt(ast);
        } else {
            expr(ast);
        }
        if (ast->next && bail == false)
            exec(ast->next);
    }
}