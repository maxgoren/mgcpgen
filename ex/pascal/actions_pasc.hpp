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
    a[0] = updateExprNode(a[0], NUM_EXPR);
    return a[0]; 
}

AST* mkId(vector<AST*>& a) {
    a[0] = updateExprNode(a[0], ID_EXPR);
    return a[0];
}

AST* mkString(vector<AST*>& a) {
    a[0] = updateExprNode(a[0], STRING_EXPR);
    return a[0]; 
}

AST* binop(vector<AST*>& reducing) {
    AST* nn = reducing[1];
    nn->attr = {EXPR_NODE,BIN_EXPR};
    nn->children[0] = reducing[0];
    nn->children[1] = reducing[2];
    return nn; 
}
AST* unary(vector<AST*>& reducing) {
    AST* nn = reducing[0];
    nn->attr = {EXPR_NODE, UNARY_EXPR};
    nn->children[0] = reducing[1];
    return nn;
}

AST* mkPrint(vector<AST*>& reducing) {
    AST* nn = reducing[0];
    nn = updateStmtNode(nn, PRINT_STMT);
    nn->children[0] = reducing[1];
    return nn;
}

AST* mkIf(vector<AST*>& reducing) {
    AST* nn = reducing[0];
    nn = updateStmtNode(nn, IF_STMT);
    nn->children[0] = reducing[1];
    nn->children[1] = reducing[3];
    nn->children[2] = reducing[5];
    delete reducing[2];
    delete reducing[4];
    return nn;
}

AST* mkWhile(vector<AST*>& reducing) {
    AST* nn = reducing[0];
    nn = updateStmtNode(nn, WHILE_STMT);
    nn->children[0] = reducing[1];
    nn->children[1] = reducing[3];
    delete reducing[2];
    return nn;
}

AST* mkBlock(vector<AST*>& reducing) {
    AST* nn = reducing[0];
    nn = updateStmtNode(nn, BLOCK_STMT);
    nn->children[0] = reducing[1];
    return nn;
}

AST* mkRet(vector<AST*>& reducing) {
    reducing[0] = updateStmtNode(reducing[0], RETURN_STMT);
    reducing[0]->children[0] = reducing[1];
    return reducing[0];
}

AST* through(vector<AST*>& a) {
    for (int i = 1; i < a.size(); i++)
        if (a[i] != nullptr)
            delete a[i];
    return a[0];
}

AST* pass(vector<AST*>& a) {
    delete a[0];
    for (int i = 2; i < a.size(); i++)
        if (a[i] != nullptr)
            delete a[i];
    return a[1];
}

bool isSeperator(TKSymbol symbol) {
    return symbol == TK_COMMA || symbol == TK_SEMI;
}

AST* mkList(vector<AST*>& reducing) {
    if (reducing[0]->token.getSymbol() == TK_LPAREN && reducing[1]->token.getSymbol() == TK_RPAREN) {
        return reducing[0];
    }
    for (int i = 1; i < reducing.size(); i++) {
            AST* itr = reducing[0];
            while (itr->next) itr = itr->next;
        itr->next = isSeperator(reducing[i]->token.getSymbol()) ? reducing[i]->children[0]:reducing[i];
    }
    return reducing[0];
}

AST* mkCall(vector<AST*>& reducing) {
    AST* nn = makeExprNode(FUNC_EXPR, reducing[0]->token);
    nn->children[0] = reducing[0];
    nn->children[0]->attr.type = EXPR_NODE;
    if (nn->children[0]->attr.expr != LAMBDA_EXPR) {
        nn->children[0] = updateExprNode(nn->children[0],ID_EXPR);
    }
    nn->children[1] = reducing[2];
    return nn;
}

AST* mkLet(vector<AST*>& reducing) {
    reducing[0] = updateStmtNode(reducing[0], LET_STMT);
    reducing[0]->children[0] = reducing[1];
    if (reducing[0]->children[0]->attr.expr != BIN_EXPR) {
        reducing[0]->children[0]->attr = {EXPR_NODE,ID_EXPR};
    }
    if (reducing.size() > 1) {
        reducing.pop_back();
        reducing[0]->children[1] = reducing.back();
    }
    return reducing[0];
}


AST* mkFunc(vector<AST*>& reducing) {
    AST* nn = reducing[1];
    nn->token.setString(reducing[0]->token.getString());
    nn->children.insert(nn->children.begin(),reducing[0]);
    nn->children[0] = updateExprNode(nn->children[0], ID_EXPR);
    nn->children[2] = reducing[2];
    AST* header = nullptr;
    for (auto it = nn->children[1]; it != nullptr; it = it->next) {
        AST* tmp = makeStmtNode(LET_STMT, it->token);
        tmp->children[0] = makeExprNode(ID_EXPR, it->token);
        if (header == nullptr) {
            header = tmp;
        } else {
            auto ri = header;
            while (ri->next != nullptr) ri = ri->next;
            ri->next = tmp;
        }
    }
    nn->children[1] = header;
    return nn;
}

AST* mkFuncHeader(vector<AST*>& reducing) {
    AST* nn = reducing[0];
    nn = updateStmtNode(nn, DEF_STMT);
    if (reducing.size() == 6) {
        nn->children[0] = reducing[1];
        nn->children[0] = updateExprNode(nn->children[0],ID_EXPR);
        nn->children[1] = reducing[2];
        delete reducing[3];
        delete reducing[4];
        delete reducing[5];
    }
    return nn;
}

AST* mkSubscript(vector<AST*>& reducing) {
    AST* nn = makeExprNode(reducing[1]->token.getSymbol() == TK_PERIOD ? FIELD_EXPR:SUBSCRIPT_EXPR, reducing[1]->token);
    nn->children[0] = reducing[0];
    nn->children[1] = reducing[2];
    delete reducing[1];
    return nn;
}

AST* mkListCon(vector<AST*>& reducing) {
    reducing[0] = updateExprNode(reducing[0], LIST_EXPR);
    if (reducing[1]->token.getSymbol() == TK_RPAREN)
        return reducing[0];
    else {
        for (int i = 1; i < reducing.size()-1; i++) {
            if (reducing[0]->children[0] == nullptr) {
                reducing[0]->children[0] = reducing[i];
            } else {
                AST* itr = reducing[0]->children[0];
                while (itr->next != nullptr) itr = itr->next;
                itr->next = reducing[i];
            }
        }
    }
    return reducing[0];
}

AST* mkProgramHeader(vector<AST*>& reducing) {
    AST* ast = reducing[0];
    ast = updateStmtNode(ast, PROGRAM_STMT);
    ast->children[0] = reducing[1];
    ast->children[0] = updateExprNode(ast->children[0],ID_EXPR);
    ast->children[1] = reducing[3];
    return ast;
}

AST* mkProgram(vector<AST*>& reducing) {
    AST* program = reducing[0];
    for (int i = 1; i < reducing.size(); i++) {
        auto it = program;
        while (it->next != nullptr) it = it->next;
        it->next = reducing[i];
    }
    return program;
}

AST* mkArray(vector<AST*>& reducing) {
    //7 Items
    // TK_ARRAY TK_LSQB EXPR TK_RANGE EXPR TK_RSQB TK_OF STANDTYPE 
    AST* nn = makeExprNode(ARRAY_CON_EXPR, reducing[0]->token);
    nn->children[0] = reducing[2];
    nn->children[1] = reducing[4];
    nn->children[2] = reducing.back();
    delete reducing[0];
    delete reducing[1];
    delete reducing[3];
    delete reducing[5];
    return nn;
}

AST* mkRecord(vector<AST*>& reducing) {
    AST* nn = makeStmtNode(RECORD_DEF_STMT, reducing[1]->token);   
    nn->children[0] = reducing[1];
    nn->children[0] = updateExprNode(nn->children[0],ID_EXPR);
    nn->children[1] = reducing[4];
    preorder(nn, 1);
    return nn;
}

AST* mkRecField(vector<AST*>& reducing) {
    AST* nn = makeStmtNode(LET_STMT, reducing[0]->token);
    nn->children[0] = reducing[0];
    nn->children[0] = updateExprNode(nn->children[0],ID_EXPR);
    nn->children[1] = reducing[2];
    return nn;
}

AST* mkTypeExpr(vector<AST*>& reducing) {
    AST* nn = updateExprNode(reducing[0], TYPE_EXPR);
    if (nn->token.getSymbol() == TK_ID) {
        nn->attr = {EXPR_NODE, BLESS_EXPR};
        nn->children[0] = makeExprNode(ID_EXPR, reducing[0]->token);
    }
    return nn;
}

#endif