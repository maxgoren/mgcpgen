#pragma once
#include "../parse/ast.hpp"
#include "object.hpp"
#include <unordered_map>
#include <iostream>
#include <vector>
#include <stack>
using namespace std;

class ReturnException : public std::exception {
    virtual const char* what() const throw() {
        return "ret stmt";
    }
};

struct Context {
    unordered_map<string, Object> globals;
    vector<unordered_map<string, Object>> symtab;
    unordered_map<string, Object> funcTab;
    stack<Object> st;
    Context() {
        symtab.push_back(unordered_map<string,Object>());
    }
    void get(string name, int d) {
        if (d == -1) {
            st.push(globals[name]);
            return;
        }
        auto it = symtab.rbegin();
        int i = 0;
        while (i < d && it != symtab.rend()) {
            i++;
            it++;
        }
        if (i != d) {
            std::cout<<"Couldn't find "<<name<<" at level "<<d<<endl;
            st.push(Object());
        } else {
            st.push((*it)[name]);
        }
    }
    void put(string name, int d, Object obj) {
        if (d == -1) {
            globals[name] = obj;
            return;
        }
        auto it = symtab.rbegin();
        int i = 0;
        while (i < d && it != symtab.rend()) {
            i++;
            it++;
        }
        if (i == d) {
            (*it)[name] = obj;
        }
    }
};

class Resolver {
    private:
        vector<unordered_map<string,bool>> scopes;
        static string blockName() {
            static int num = 0;
            return "block"+to_string(num++);
        }
        static string lambdaName() {
            static int num = 0;
            return "lambda"+to_string(num++);
        }
        void openScope() {
            scopes.push_back(unordered_map<string,bool>());
        }
        void closeScope() {
            scopes.pop_back();
        }
        void resolveName(AST* ast, string name) {
            if (scopes.empty())
                return;
            int i = 0;
            auto it = scopes.rbegin();
            while (it != scopes.rend()) {
                if ((*it).find(name) != (*it).end()) {
                    ast->token.setScopeLevel(scopes.size() - 1 - i);
                    cout<<name<<" at "<< scopes.size() - 1 - i <<endl;
                    return;
                }
                it++;
                i++;
            }
            cout<<name<<" aint got no gas in it!"<<endl;
            ast->token.setScopeLevel(-1);
        }
        void declareName(string name) {
            if (scopes.empty())
                return;
            if (scopes.back().find(name) != scopes.back().end()) {
                cout<<"Attempted to declare already declared name: "<<name<<endl;
                return;
            }
            scopes.back()[name] = true;
        }
        void resolveStmt(AST* ast) {
            if (ast == nullptr)
                return;
            switch (ast->attr.stmt) {
                case LET_STMT: {
                    resolveExpr(ast->children[0], true);
                } break;
                case EXPR_STMT:
                case RETURN_STMT:
                case PRINT_STMT:
                    resolveExpr(ast->children[0], false);
                    break;
                case IF_STMT: {
                    resolveExpr(ast->children[0], false);
                    resolve(ast->children[1]);
                    resolve(ast->children[2]);
                } break;
                case WHILE_STMT: {
                    resolveExpr(ast->children[0], false);
                    resolve(ast->children[1]);
                } break;
                case BLOCK_STMT: {
                    openScope();
                    for (auto it = ast->children[0]; it != nullptr; it = it->next)
                        resolve(it);
                    closeScope();
                } break;
                case DEF_STMT: {
                    declareName(ast->children[0]->token.getString());   
                    openScope();
                    for (auto i = 1; i < 3; i++) {
                        resolve(ast->children[i]);
                    }
                    closeScope();
                } break;
                default:
                    break;
            }
            resolve(ast->next);
        }
        void resolveExpr(AST* ast, bool fromLet) {
            if (ast == nullptr)
                return;
            switch (ast->attr.expr) {
                case ID_EXPR: {
                    if (fromLet) {
                        cout<<"Coming from Let, declaring "<<ast->token.getString()<<endl;
                        declareName(ast->token.getString());
                    }
                    cout<<"Resolving "<<ast->token.getString()<<endl;
                    resolveName(ast, ast->token.getString());
                } break;
                default:
                    for (int i = 0; i < 3; i++) {
                        resolveExpr(ast->children[i], fromLet);
                    }
            }
            resolve(ast->next);
        }
    public:
        Resolver() {

        }
        void resolve(AST*& ast) {
            if (ast == nullptr)
                return;
            cout<<nodeTypeStr[ast->attr.type]<<" ";
            if (ast->attr.type == EXPR_NODE) {
                cout<<exprTypeStr[ast->attr.expr]<<endl;
                resolveExpr(ast, false);
            } else {
                cout<<stmtTypeStr[ast->attr.expr]<<endl;
                resolveStmt(ast);
            }
        }
};

class Interpreter {
    private:
        Context cxt;
        bool isbuiltin(string name);
        void dobuiltin(AST* ast);
        void evalParams(AST* params, AST* args);
        void evalLambdaFunc(AST* params, AST* args, AST* body);
        void stmt(AST* ast);
        void eval(AST* ast);
        void evalBinOp(AST* ast);
    public:
        Interpreter() {
            cxt.symtab.push_back(unordered_map<string,Object>());
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



void Interpreter::evalParams(AST* params, AST* args) {
    unordered_map<string, Object> tmp;
    while (params != nullptr && args != nullptr) {
        string nm = params->children[0]->token.getString();
        eval(args);
        tmp[nm] = cxt.st.top(); cxt.st.pop();
        cout<<"Assigned "<<tmp[nm].toString()<<" to "<<nm<<endl;
        params = params->next;
        args = args->next;
    }
    cxt.symtab.push_back(tmp);
}

void Interpreter::evalLambdaFunc(AST* params, AST* args, AST* body) {
    evalParams(params, args);
    try {
        exec(body);
    } catch (ReturnException re) {

    }
    cxt.symtab.pop_back();
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
                    t = cxt.funcTab[fname];
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
            cxt.put(ast->children[0]->token.getString(), ast->children[0]->token.scopeLevel(), cxt.st.top());
        } else if (ast->children[0]->attr.expr == SUBSCRIPT_EXPR) {
            eval(ast->children[0]->children[0]);
            eval(ast->children[0]->children[1]);
            Object idx = cxt.st.top(); cxt.st.pop();
            Object lval = cxt.st.top(); cxt.st.pop();
            lval.listval->at(idx.numval) = cxt.st.top(); cxt.st.pop();
            cxt.symtab.back()[ast->children[0]->children[0]->token.getString()] = lval;
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
        case BLOCK_STMT: {
            cxt.symtab.push_back(unordered_map<string, Object>());
            exec(ast->children[0]);
            cxt.symtab.pop_back();
        } break;
        case IF_STMT:
            eval(ast->children[0]);
            if (cxt.st.top().boolval) {
                exec(ast->children[1]);
            }
        break;
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