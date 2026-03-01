#include <fstream>
#include <iostream>
#include "../../src/cfg.hpp"
#include "../../src/ParserGenerator.hpp"
#include "lexer.hpp"
#include "parse.hpp"
using namespace std;

void interpreter(Grammar& G, ParseTable& table, string expr) {
    Lexer lexer;
    Parser parser(G, table);
    string inbuff = expr;
    StringBuffer* sb = new StringBuffer();
    PrintVisitor* pv = new PrintVisitor();
    EvalVisitor* ev  = new EvalVisitor();
    do {
        cout<<"\n -=> ";
        getline(cin, inbuff);
        sb->init(inbuff);
        vector<Token> tokens = lexer.lex(sb);
        AST* ast = parser.parse(tokens,"stmt-seq");
        if (ast != nullptr && dynamic_cast<StmtSequence*>(ast) != nullptr) {
            pv->visit(dynamic_cast<StmtSequence*>(ast)), ev->visit(dynamic_cast<StmtSequence*>(ast));
        } else if (ast != nullptr && dynamic_cast<ExprStmt*>(ast) != nullptr) {
            pv->visit(dynamic_cast<ExprStmt*>(ast)), ev->visit(dynamic_cast<ExprStmt*>(ast));
        } else if (ast != nullptr && dynamic_cast<WhileStmt*>(ast) != nullptr) {
            pv->visit(dynamic_cast<WhileStmt*>(ast)), ev->visit(dynamic_cast<WhileStmt*>(ast));
        } else if (ast != nullptr && dynamic_cast<LetStmt*>(ast) != nullptr) {
            pv->visit(dynamic_cast<LetStmt*>(ast)), ev->visit(dynamic_cast<LetStmt*>(ast));
        } else if (ast != nullptr && dynamic_cast<PrintStmt*>(ast) != nullptr) {
            pv->visit(dynamic_cast<PrintStmt*>(ast)), ev->visit(dynamic_cast<PrintStmt*>(ast));
        }
    } while (inbuff != "quit");
}

void showUsage(string name) {
    cout<<"MGCPGen (c) 2026 MaxGCoding.com"<<endl;
    cout<<name<<" <grammar file>"<<endl;
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        showUsage(argv[0]);
        return 1;
    }
    Grammar G;
    G.readGrammarFile(argv[1]);
    ParserGenerator pg;
    ParseTable table = pg.generate(G, "stmt-seq");
    interpreter(G, table, "prev := 0; curr := 1; while (curr < 540) { next := prev + curr; print next; prev := curr; curr := next; };");
    return 0;
}