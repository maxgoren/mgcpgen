#include "slr_gen.hpp"
#include "slr_parser.hpp"

void showUsage(string name) {
    cout<<"MGCPGen, SLR Generator"<<endl;
    cout<<name<<" <filename> "<<endl;
}

void parseWith(Grammar& G, ActionTable& actTab, GoToTable& goTab, vector<LRState>& states) {
    Lexer lexer;
    StringBuffer* sb = new StringBuffer();
    string expr;
    SLRParser parser(actTab, goTab, states);
    do {
        cout<<"?> ";
        getline(cin, expr);
        sb->init(expr);
        vector<Token> tokens = lexer.lex(sb);
        cout<<"Parsing: "<<expr<<endl;
        AST* ast = parser.shift_reduce_driver(G, tokens);
        preorder(ast,1);
        exec(ast);
    } while (true);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        showUsage(argv[0]);
        return 1;
    }
    Grammar G;
    G.readGrammarFile(argv[1]);
    SLRGenerator slrgen;
    auto[actTab, goTab] = slrgen.generate(G, "sp");
    parseWith(G, actTab, goTab, slrgen.getStates());
    return 0;
}