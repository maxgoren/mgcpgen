#include "slr_parser.hpp"
#include "terp.hpp"

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
        AST* ast = parser.shift_reduce_driver(tokens);
        preorder(ast,1);
        exec(ast);
    } while (true);
    return 0;
}