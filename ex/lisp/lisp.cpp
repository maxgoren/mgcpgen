#include "../../src/ParserGenerator.hpp"
#include "lisp_parse.hpp"
using namespace std;

void interpreter(Grammar& G, ParseTable& table, string expr);

ParseTable lispGrammar() {
    Grammar G;
    G.terminals = {"TK_LPAREN", "TK_RPAREN", "TK_QUOTE", "TK_ID", "TK_NUM", "TK_PLUS", "TK_MINUS", "TK_MUL", "TK_PRINT", "$$", EPS };
    G.nonterminals = {"sexpr", "atom", "list", "built-in"};

    G.productions["sexpr"] = ProductionSet({Production(1,"sexpr", SymbolString({"atom"})), 
                                            Production(2, "sexpr", SymbolString({"TK_LPAREN", "list", "TK_RPAREN"})),
                                        Production(10, "sexpr", SymbolString({"TK_QUOTE", "sexpr"}))});
    G.productions["list"] = ProductionSet({Production(8, "list", SymbolString({"sexpr", "list"})),
                                            Production(9, "atom", SymbolString({EPS})) });
    G.productions["atom"] = ProductionSet({Production(3, "atom", SymbolString({"TK_ID"})),
                                            Production(4, "atom", SymbolString({"TK_NUM"})),
                                            Production(5, "atom", SymbolString({"built-in"}))});
    G.productions["built-in"] = ProductionSet({Production(6, "built-in", SymbolString({"TK_PLUS"})),
                                            Production(7, "built-in", SymbolString({"TK_MINUS"})),
                                            Production(8, "built-in", SymbolString({"TK_MUL"}))});
   
    ParserGenerator pg;
    ParseTable table = pg.generate(G, "list");
    pg.tableGen.persist("lisp.gmpt");
    interpreter(G, table, "(+ 1 2)");
    return table;
}

void interpreter(Grammar& G, ParseTable& table, string expr) {
    Lexer lexer;
    Parser parser(G, table);
    string inbuff = expr;
    StringBuffer* sb = new StringBuffer();
    do {
        sb->init(inbuff);
        vector<Token> tokens = lexer.lex(sb);
        AST* ast = parser.parse(tokens,"list");
        if (ast != nullptr) ast->print();
        cout<<"\n -=> ";
        getline(cin, inbuff);
    } while (true);
}

int main(int argc, char* argv[]) {
    ParseTable pt = lispGrammar();
    return 0;
}