#include <fstream>
#include <iostream>
#include "lexer.hpp"
#include "parse.hpp"
#include "visitors.hpp"
#include "hellapint.h"
using namespace std;

AST* stringToAST(string input, Lexer lexer, Parser parser) {
    StringBuffer* sb = new StringBuffer();
    sb->init(input);
    vector<Token> tokens = lexer.lex(sb);
    return parser.parse(tokens, "stmt-seq");
}

void interpreter(ParseTable& table, string expr) {
    Lexer lexer;
    Parser parser(table, terminalSymbols, nonTerminalSymbols);
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


int main(int argc, char* argv[]) {
    initParseTable();
    interpreter(parseTable, "prev := 0; curr := 1; while (curr < 150) { next := prev + curr; print next; prev := curr; curr := next; 1 }; def eko(let a) { print a+a }; eko(13)");
    return 0;
}