#include "cfg.hpp"
#include "calc_firsts.hpp"
#include "calc_follows.hpp"
#include "build_table.hpp"
#include "recognize.hpp"

// LL(1) Expression Grammar Taken from Dragon Book
void expGramWithStmts(string expr) {
    Grammar G;
    G.nonterminals = { "stmt-seq", "stmt-seqT", "stmt", "print-stmt", "print-stmtT", "expr-stmt", "expr", "exprT", "term", "termT", "factor"};
    G.terminals = { "TK_ID", "TK_NUM", "TK_LPAREN", "TK_RPAREN", "TK_PRINT", "TK_PLUS", "TK_MUL","TK_MINUS", "TK_DIV", "TK_SEMI", "$$", EPS };
    G.productions["stmt-seq"] =     ProductionSet({Production(1,"stmt-seq",SymbolString({"stmt", "stmt-seqT"}))});

    G.productions["stmt-seqT"] =    ProductionSet({Production(2,"stmt-seqT",SymbolString({ "TK_SEMI", "stmt-seq"})), 
                                                   Production(3,"stmt-seqT",SymbolString({"#"}))});

    G.productions["stmt"] =         ProductionSet({Production(4,"stmt",SymbolString({"expr-stmt"})), 
                                                   Production(5,"stmt",SymbolString({"print-stmt"}))});

    G.productions["print-stmt"] =   ProductionSet({Production(6,"print-stmt",SymbolString({"TK_PRINT", "print-stmtT"}))});
    G.productions["print-stmtT"] = ProductionSet({Production(7,"print-stmtT",SymbolString({"expr"}))});
    G.productions["expr-stmt"] =    ProductionSet({Production(8,"expr-stmt",SymbolString({"expr"}))});
    
    G.productions["expr"] =         ProductionSet({Production(9,"expr",SymbolString({"term", "exprT"}))});
    
    G.productions["exprT"] =        ProductionSet({Production(10,"exprT",SymbolString({"TK_PLUS", "term", "exprT"})), 
                                                   Production(11,"exprT",SymbolString({"TK_MINUS", "term", "exprT"})), 
                                                   Production(12,"exprT",SymbolString({"#"}))});

    G.productions["term"] =         ProductionSet({Production(13,"term",SymbolString({"factor","termT"}))});
    
    G.productions["termT"] =        ProductionSet({Production(14,"termT",SymbolString({"TK_MUL", "factor", "termT"})), 
                                                   Production(15,"termT",SymbolString({"TK_DIV", "factor", "termT"})), 
                                                   Production(16,"termT",SymbolString({"#"}))});

    G.productions["factor"] =       ProductionSet({Production(17,"factor",SymbolString({"TK_NUM"})), 
                                                   Production(18,"factor",SymbolString({"TK_ID"})), 
                                                   Production(19,"factor",SymbolString({"TK_LPAREN", "expr", "TK_RPAREN"}))});
    calcFirstSets(G);
    calcFollowSets(G, "stmt-seq");
    printFirsts(G);
    printFollows(G);
    auto table = makeParseTable(G);
    Lexer lexer;
    StringBuffer* sb = new StringBuffer();
    sb->init(expr);
    vector<Token> tokens = lexer.lex(sb);
    recognize(tokens, G, table, "stmt-seq");
}

int main(int argc, char* argv[]) {
    expGramWithStmts(argv[1]);
    return 0;
}
