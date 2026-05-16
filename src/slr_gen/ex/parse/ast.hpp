#pragma once
#include "../lex/token.hpp"
#include <iostream>
using namespace std;

enum NODE_TYPE {
    BIN_EXPR, UNARY_EXPR, ID_EXPR, NUM_EXPR, STRING_EXPR, FUNC_EXPR, LIST_EXPR, SUBSCRIPT_EXPR,
    IF_STMT, PRINT_STMT, DEF_STMT, EXPR_STMT, BLOCK_STMT, LET_STMT, RETURN_STMT, TMP
};

string nodeTypeStr[] = {
    "BIN_EXPR", "UNARY_EXPR", "ID_EXPR", "NUM_EXPR", "STRING_EXPR", "FUNC_EXPR", "LIST_EXPR", "SUBSCRIPT_EXPR",
    "IF_STMT", "PRINT_STMT", "DEF_STMT", "EXPR_STMT", "BLOCK_STMT", "LET_STMT", "RETURN_STMT", "TEMP NODE"
};

struct AST {
    NODE_TYPE type;
    Token token;
    AST* next;
    vector<AST*> children;
    AST(Token& tk) : token(tk), children(vector<AST*>(3)), next(nullptr) { type = TMP; }
    AST() { 
        children = vector<AST*>(3);
        next = nullptr;
        type = TMP;
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