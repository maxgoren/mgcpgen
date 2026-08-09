#include "parse/lr_parser.hpp"
#include "lex/lexer.hpp"
#include "interp/terp.hpp"

void terp() {
    Lexer lexer;
    StringBuffer* sb = new StringBuffer();
    string expr;
    LRParser parser;
    Interpreter terp;
    do {
        cout<<"?> ";
        getline(cin, expr);
        sb->init(expr);
        vector<Token> tokens = lexer.lex(sb);
        cout<<"Parsing: "<<expr<<endl;
        AST* ast = parser.parse(tokens);
        preorder(ast,1);
        terp.exec(ast);
    } while (true);
}

void runfile(string filename) {
    FileStringBuffer* fsb = new FileStringBuffer();
    if (fsb->readFile(filename)) {
        Lexer lexer;
        LRParser parser;
        Interpreter interpreter;
        ScopeResolution sr;
        auto tokens = lexer.lex(fsb);
        auto ast = parser.parse(tokens);
        sr.resolveScopes(ast);
        preorder(ast, 1);
        interpreter.exec(ast); 
    }
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		terp();
	}
    runfile(argv[1]);
    return 0;
}


