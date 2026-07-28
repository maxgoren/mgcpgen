#ifndef ast_hpp
#define ast_hpp
#include <map>
#include <unordered_set>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include "ast.hpp"
using namespace std;

AST* makebinop(vector<AST*>& reducing) {
    AST* nn = makeExprNode(BIN_EXPR, reducing[1]->token);
    nn->children[0] = reducing[0];
    nn->children[1] = reducing[2];
    return nn; 
}
AST* makeunary(vector<AST*>& reducing) {
    AST* nn = makeExprNode(UNARY_EXPR, reducing[0]->token);
    nn->children[0] = reducing[1];
    return nn;
}

AST* makelist(vector<AST*>& reducing) {
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

AST* makeprint(vector<AST*>& reducing) {
    AST* nn = makeStmtNode(PRINT_STMT,reducing[0]->token);
    nn->children[0] = reducing[1];
    return nn;
}

AST* makeIf(vector<AST*>& reducing) {
    AST* nn = makeStmtNode(IF_STMT,reducing[0]->token);
    nn->children[0] = reducing[2];
    nn->children[1] = reducing[4];
    return nn;
}

AST* makeWhile(vector<AST*>& reducing) {
    AST* nn = makeStmtNode(WHILE_STMT, reducing[0]->token);
    nn->children[0] = reducing[2];
    nn->children[1] = reducing[4];
    return nn;
}

AST* makeBlock(vector<AST*>& reducing) {
    AST* nn = makeStmtNode(BLOCK_STMT, reducing[0]->token);
    nn->children[0] = reducing[1];
    return nn;
}

AST* makeCall(vector<AST*>& reducing) {
    cout<<"Make call from: ";
    for (auto m : reducing) {
        cout<<m->token.getString()<<endl;
    }
    AST* nn = makeExprNode(FUNC_EXPR, reducing[0]->token);
    nn->children[0] = reducing[0];
    nn->children[0]->attr.type = EXPR_NODE;
    if (nn->children[0]->attr.expr != LAMBDA_EXPR) {
        nn->children[0]->attr.expr = ID_EXPR;
    }
    nn->children[1] = reducing[2];
    return nn;
}

AST* makeLet(vector<AST*>& reducing) {
    reducing[0]->attr.type = STMT_NODE;
    reducing[0]->attr.stmt = LET_STMT;
    reducing[0]->children[0] = reducing[1];
    if (reducing[0]->children[0]->attr.expr != BIN_EXPR) {
        reducing[0]->children[0]->attr.type = EXPR_NODE;
        reducing[0]->children[0]->attr.expr = ID_EXPR;
    }
    return reducing[0];
}

AST* makeRet(vector<AST*>& reducing) {
    reducing[0]->attr.type = STMT_NODE;
    reducing[0]->attr.stmt = RETURN_STMT;
    reducing[0]->children[0] = reducing[1];
    return reducing[0];
}


AST* makeFunc(vector<AST*>& reducing) {
    AST* nn = reducing[0];
    nn->attr.type = STMT_NODE;
    nn->attr.stmt = DEF_STMT;
    if (reducing.size() == 6) {
        nn->children[0] = reducing[1];
        nn->children[0]->attr.type = EXPR_NODE;
        nn->children[0]->attr.expr = ID_EXPR;
        nn->children[1] = reducing[3];
        nn->children[2] = reducing[5];
        for (auto m = nn->children[1]; m != nullptr; m = m->next) {
            if (m->token.getSymbol() == TK_LET) {
                m->children[0]->attr.type = EXPR_NODE;
                m->children[0]->attr.expr = ID_EXPR;
            }
        }
    } else {
        cout<<"Nah man."<<endl;
    }
    return nn;
}

AST* makeStruct(vector<AST*>& reducing) {
    AST* nn = reducing[0];
    nn->attr.type = STMT_NODE;
    nn->attr.stmt = STRUCT_STMT;
    reducing[1]->attr.type = EXPR_NODE;
    reducing[1]->attr.expr = ID_EXPR;
    nn->children[0] = reducing[1];
    nn->children[1] = reducing[2];
    return nn;
}

AST* makeLambda(vector<AST*>& reducing) {
    AST* nn = reducing[0];
    nn->attr.type = EXPR_NODE;
    nn->attr.expr = LAMBDA_EXPR;
    cout<<"Make lambda from ";
    for (auto m : reducing) {
        cout<<tokenStr[m->token.getSymbol()]<<" "<< m->token.getString()<<endl;
    }
    Token tk;
    tk.setString("lambda");
    tk.setSymbol(TK_ID);
        nn->children[0] = new AST(tk);
        nn->children[0]->attr.type = EXPR_NODE;
        nn->children[0]->attr.expr = ID_EXPR;
        nn->children[1] = reducing[2];
        nn->children[2] = reducing[5];
        for (auto m = nn->children[1]; m != nullptr; m = m->next) {
            if (m->token.getSymbol() == TK_LET) {
                m->children[0]->attr.type = EXPR_NODE;
                m->children[0]->attr.expr = ID_EXPR;
            }
        }
    return nn;
}

AST* makeSubScript(vector<AST*>& reducing) {
    AST* nn = makeExprNode(SUBSCRIPT_EXPR, reducing[1]->token);
    nn->children[0] = reducing[0];
    nn->children[1] = reducing[2];
    return nn;
}

AST* makeListConstructor(vector<AST*>& reducing) {
    reducing[0]->attr.type = EXPR_NODE;
    reducing[0]->attr.expr = LIST_EXPR;
    if (reducing[1]->token.getSymbol() == TK_RPAREN)
        return reducing[0];
    else {
        for (int i = 1; i < reducing.size()-1; i++) {
            if (reducing[0]->children[0] == nullptr) {
                reducing[0]->children[0] = reducing[1];
            } else {
                AST* itr = reducing[0]->children[0];
                while (itr->next != nullptr) itr = itr->next;
                itr->next = reducing[i];
            }
        }
    }
    return reducing[0];
}

#endif