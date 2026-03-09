#include <fstream>
#include <iostream>
#include "lexer.hpp"
#include "parse.hpp"
#include "visitors.hpp"
#include "exprs.hpp"
using namespace std;

AST* stringToAST(string input, Lexer lexer, Parser parser) {
    StringBuffer* sb = new StringBuffer();
    sb->init(input);
    vector<Token> tokens = lexer.lex(sb);
    return parser.parse(tokens, "stmt");
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
    interpreter(parseTable, "1+2+3+4+5");
    return 0;
}