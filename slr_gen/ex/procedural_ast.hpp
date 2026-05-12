#ifndef ast_hpp
#define ast_hpp
#include "../token.hpp"
#include <map>
#include <unordered_set>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include "object.hpp"
using namespace std;
struct AST {
    Token token;
    AST* next;
    vector<AST*> children;
    AST(Token& tk) : token(tk), children(vector<AST*>(3)), next(nullptr) { }
    AST() { }
};
using Symbol = string;
using GoToTable = map<int,map<Symbol,int>>;
using ActionTable = map<int,map<Symbol,string>>;

void preorder(AST* ast, int d) {
    if (ast != nullptr) {
        for (int i = 0; i < d; i++) cout<<" ";
        cout<<ast->token.getString()<<endl;
        for (AST* n : ast->children) {
            preorder(n, d+1);
        }
        preorder(ast->next, d);
    }
}
unordered_map<string, Object> symtab;

Object eval(AST* ast) {
    if (ast != nullptr) {
        eval(ast->next);
        if (ast->children[0] == nullptr) {
            cout<<"Leaf Node: "<<ast->token.getString()<<endl;
            if (ast->token.getSymbol() == TK_NUM) {
                return Object(stod(ast->token.getString()));
            } else if (ast->token.getSymbol() == TK_ID) {
                return symtab[ast->token.getString()];
            } else if (ast->token.getSymbol() == TK_STRING) {
                return Object(new string(ast->token.getString()));
            } else {
                return Object(0.0);
            }
        } else {
            if (ast->token.getSymbol() == TK_ASSIGN) {
                symtab[ast->children[0]->token.getString()] = eval(ast->children[1]);
                return symtab[ast->children[0]->token.getString()];
            }
            Object lho = eval(ast->children[0]);
            Object rho = eval(ast->children[1]);
            double lhs = lho.numval;
            double rhs = rho.numval;
            switch (ast->token.getSymbol()) {
                case TK_MINUS: {
                    if (ast->children[1] == nullptr) {
                        return -lhs;
                    } else {
                        return lhs-rhs;
                    }
                } break;
                case TK_PLUS: return lhs+rhs;
                case TK_MUL:  return lhs*rhs;
                case TK_DIV:  return lhs/rhs;
                case TK_NUM:  return stod(ast->token.getString());
                case TK_EQU:  return lhs == rhs;
                case TK_NEQ:  return lhs != rhs;
                case TK_LT:   return lhs < rhs;
                case TK_GT:   return lhs > rhs;
            }
        }
    }
    return 0.0;
}

Object exec(AST* ast) {
    Object val = 0.0;
    if (ast != nullptr) {
        if (ast->token.getSymbol() == TK_PRINT) {
            cout<<eval(ast->children[0]).toString()<<endl;
        } else if (ast->token.getSymbol() == TK_LCURLY) {
            exec(ast->children[0]);
        } else if (ast->token.getSymbol() == TK_IF) {

        } else {
            val = eval(ast);
        }
        if (ast->next)
            exec(ast->next);
    }
    return val;
}

AST* makebinop(vector<AST*>& reducing) {
    AST* nn = reducing[1];
    nn->children[0] = reducing[0];
    nn->children[1] = reducing[2];
    return nn; 
}
AST* makeunary(vector<AST*>& reducing) {
    AST* nn = new AST(reducing[0]->token);
    nn->children[0] = reducing[1];
    return nn;
}

AST* makelist(vector<AST*>& reducing) {
    AST* itr = reducing[0];
    while (itr->next) itr = itr->next;
    itr->next = reducing[1];
    return reducing[0];
}

AST* makeprint(vector<AST*>& reducing) {
    AST* nn = new AST(reducing[0]->token);
    nn->children[0] = reducing[1];
    return nn;
}

AST* makeIf(vector<AST*>& reducing) {
    AST* nn = new AST(reducing[0]->token);
    cout<<"Gotta string together these: \n";
    for (auto m : reducing) {
        preorder(m, 1);
        cout<<"\n---------\n";
    }
    return nn;
}

AST* makeBlock(vector<AST*>& reducing) {
    AST* nn = new AST(reducing[0]->token);
    nn->children[0] = reducing[1];
    return nn;
}

AST* makeFunc(vector<AST*>& reducing) {
    return reducing[0];
}

#endif