#include <fstream>
#include <iostream>
#include "../../src/cfg.hpp"
#include "../../src/ParserGenerator.hpp"
#include "lexer.hpp"
#include "parse.hpp"
#include "visitors.hpp"
using namespace std;

AST* stringToAST(string input, Lexer lexer, Parser parser) {
    StringBuffer* sb = new StringBuffer();
    sb->init(input);
    vector<Token> tokens = lexer.lex(sb);
    return parser.parse(tokens, "stmt-seq");
}

void interpreter(Grammar& G, ParseTable& table, string expr) {
    Lexer lexer;
    Parser parser(G, table);
    string inbuff = expr;
    StringBuffer* sb = new StringBuffer();
    PrintVisitor* pv = new PrintVisitor();
    EvalVisitor* ev  = new EvalVisitor();
    do {
        AST* ast = stringToAST(inbuff, lexer, parser);
        pv->print(ast);
        ev->eval(ast);
        cout<<"\n -=> ";
        getline(cin, inbuff);
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
    interpreter(G, table, "prev := 0; curr := 1; while (curr < 150) { next := prev + curr; print next; prev := curr; curr := next; next }");
    return 0;
}