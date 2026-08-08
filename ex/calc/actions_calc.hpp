#ifndef ast_hpp
#define ast_hpp
#include <map>
#include <unordered_set>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include "ast.hpp"
using namespace std;

AST* mkNum(vector<AST*>& a) {
    a[0]->attr.type = EXPR_NODE; 
    a[0]->attr.expr = NUM_EXPR; 
    return a[0]; 
}

AST* through(vector<AST*>& a) {
    return a[0];
}

AST* pass(vector<AST*>& a) {
    return a[1];
}

AST* mkId(vector<AST*>& a) {
    a[0]->attr.type = EXPR_NODE; 
    a[0]->attr.expr = ID_EXPR; 
    return a[0];
}

AST* binop(vector<AST*>& reducing) {
    AST* nn = makeExprNode(BIN_EXPR, reducing[1]->token);
    nn->children[0] = reducing[0];
    nn->children[1] = reducing[2];
    return nn; 
}
AST* unary(vector<AST*>& reducing) {
    AST* nn = makeExprNode(UNARY_EXPR, reducing[0]->token);
    nn->children[0] = reducing[1];
    return nn;
}

AST* mkList(vector<AST*>& reducing) {
    if (reducing[0]->token.getSymbol() == TK_LPAREN && reducing[1]->token.getSymbol() == TK_RPAREN) {
        return reducing[0];
    }
    for (int i = 1; i < reducing.size(); i++) {
            AST* itr = reducing[0];
            while (itr->next) itr = itr->next;
            itr->next = reducing[i]->token.getSymbol() == TK_COMMA ? reducing[i]->children[0]:reducing[i];
    }
    return reducing[0];
}

AST* mkPrint(vector<AST*>& reducing) {
    AST* nn = makeStmtNode(PRINT_STMT,reducing[0]->token);
    nn->children[0] = reducing[1];
    return nn;
}


AST* mkLet(vector<AST*>& reducing) {
    reducing[0]->attr.type = STMT_NODE;
    reducing[0]->attr.stmt = LET_STMT;
    reducing[0]->children[0] = reducing[1];
    if (reducing[0]->children[0]->attr.expr != BIN_EXPR) {
        reducing[0]->children[0]->attr.type = EXPR_NODE;
        reducing[0]->children[0]->attr.expr = ID_EXPR;
    }
    return reducing[0];
}

#endif