#pragma once
#include "../token.hpp"
using namespace std;

enum NODE_TYPE {
    BIN_EXPR, UNARY_EXPR, ID_EXPR, NUM_EXPR, STRING_EXPR, FUNC_EXPR, LIST_EXPR,
    IF_STMT, PRINT_STMT, DEF_STMT, EXPR_STMT, BLOCK_STMT, LET_STMT, RETURN_STMT, TMP
};

string nodeTypeStr[] = {
    "BIN_EXPR", "UNARY_EXPR", "ID_EXPR", "NUM_EXPR", "STRING_EXPR", "FUNC_EXPR", "LIST_EXPR",
    "IF_STMT", "PRINT_STMT", "DEF_STMT", "EXPR_STMT", "BLOCK_STMT", "LET_STMT", "RETURN_STMT"
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