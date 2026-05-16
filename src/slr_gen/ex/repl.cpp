#include "parse/slr_parser.hpp"
#include "lex/lexer.hpp"
#include "interp/terp.hpp"

int main() {
    Lexer lexer;
    StringBuffer* sb = new StringBuffer();
    string expr;
    SLRParser parser;
    do {
        cout<<"?> ";
        getline(cin, expr);
        sb->init(expr);
        vector<Token> tokens = lexer.lex(sb);
        cout<<"Parsing: "<<expr<<endl;
        AST* ast = parser.parse(tokens);
        preorder(ast,1);
        exec(ast);
    } while (true);
    return 0;
}