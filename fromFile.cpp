#include <fstream>
#include <iostream>
#include "src/cfg.hpp"
#include "src/ParserGenerator.hpp"
#include "ex/algol_like/lexer.hpp"
#include "ex/algol_like/parse.hpp"
using namespace std;

void interpreter(Grammar& G, ParseTable& table, string expr) {
    Lexer lexer;
    Parser parser(G, table);
    string inbuff = expr;
    StringBuffer* sb = new StringBuffer();
    PrintVisitor* pv = new PrintVisitor();
    EvalVisitor* ev  = new EvalVisitor();
    do {
        sb->init(inbuff);
        vector<Token> tokens = lexer.lex(sb);
        StmtSequence* ast = (StmtSequence*)parser.parse(tokens,"stmt-seq");
        if (ast != nullptr) pv->visit(ast), ev->visit(ast);
        cout<<"\n -=> ";
        getline(cin, inbuff);
    } while (true);
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
    pg.tableGen.persist("test.gmpt", G);
    interpreter(G, table, "prev := 0; curr := 1; while (curr < 40) { next := prev + curr; print next; prev := curr; curr := next; };");
    return 0;
}