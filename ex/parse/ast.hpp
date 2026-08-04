#pragma once
#include "../lex/token.hpp"
#include <iostream>
using namespace std;

enum NODE_TYPE {
    EXPR_NODE, STMT_NODE, TEMP_NODE
};

enum EXPR_TYPE {
    BIN_EXPR, UNARY_EXPR, ID_EXPR, NUM_EXPR, STRING_EXPR, FUNC_EXPR, LIST_EXPR, SUBSCRIPT_EXPR, LAMBDA_EXPR, BLESS_EXPR
};

enum STMT_TYPE {  
    IF_STMT, WHILE_STMT, PRINT_STMT, DEF_STMT, EXPR_STMT, BLOCK_STMT, STRUCT_STMT, LET_STMT, RETURN_STMT
};


string nodeTypeStr[] = {
    "EXPR_NODE", "STMT_NODE", "TEMP NODE"
};
string exprTypeStr[] = {
    "BIN_EXPR", "UNARY_EXPR", "ID_EXPR", "NUM_EXPR", "STRING_EXPR", "FUNC_EXPR", "LIST_EXPR", "SUBSCRIPT_EXPR", "LAMBDA_EXPR", "BLESS_EXPR"
};
string stmtTypeStr[] = {
    "IF_STMT", "WHILE_STMT", "PRINT_STMT", "DEF_STMT", "EXPR_STMT", "BLOCK_STMT", "STRUCT_STMT", "LET_STMT", "RETURN_STMT"
};

struct AST {
    struct Attr {
        NODE_TYPE type;
        union {
            EXPR_TYPE expr;
            STMT_TYPE stmt;
        };
    } attr;
    Token token;
    AST* next;
    vector<AST*> children;
    AST(Token& tk) : token(tk), children(vector<AST*>(3)), next(nullptr) { attr.type = TEMP_NODE; }
    AST() { 
        children = vector<AST*>(3);
        next = nullptr;
        attr.type = TEMP_NODE;
    }
};

void preorder(AST* ast, int d) {
    if (ast != nullptr) {
        for (int i = 0; i < d; i++) cout<<" ";
        cout<<nodeTypeStr[ast->attr.type]<<" ";
        if (ast->attr.type == EXPR_NODE) {
            cout<<exprTypeStr[ast->attr.expr]<<" ";
        } else if (ast->attr.type == STMT_NODE) {
            cout<<stmtTypeStr[ast->attr.stmt]<<" ";
        } else {
            cout<<" [tmp] ";
        }
        cout<<ast->token.getString();
        if (ast->attr.type == EXPR_NODE)
            cout<<" scope "<< ast->token.scopeLevel();
        cout<<endl;
        for (AST* n : ast->children) {
            preorder(n, d+1);
        }
        preorder(ast->next, d);
    }
}

AST* makeExprNode(EXPR_TYPE type, Token& t) {
    AST* ast = new AST(t);
    ast->attr.type = EXPR_NODE;
    ast->attr.expr = type;
    return ast;
}

AST* makeStmtNode(STMT_TYPE type, Token& t) {
    AST* ast = new AST(t);
    ast->attr.type = STMT_NODE;
    ast->attr.stmt = type;
    return ast;
}