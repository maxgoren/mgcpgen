#include "src/cfg.hpp"
#include "src/calc_firsts.hpp"
#include "src/calc_follows.hpp"
#include "src/build_table.hpp"
#include "src/parse.hpp"

// LL(1) Expression Grammar Taken from Dragon Book
void expGramWithStmts(string expr) {
    Grammar G;
    G.nonterminals = { "stmt-seq", 
                        "stmt-seqT", 
                        "stmt", 
                        "while-stmt",
                        "while-stmtT",
                        "print-stmt", 
                        "print-stmtT", 
                        "expr-stmt", 
                        "expr", 
                        "exprT", 
                        "rel-expr",
                        "rel-exprT",
                        "term", 
                        "termT", 
                        "factor", 
                        "primary"
                    };
    G.terminals = { "TK_ID", "TK_NUM", 
        "TK_LPAREN", "TK_RPAREN", "TK_LCURLY", "TK_RCURLY",
        "TK_PRINT","TK_WHILE", "TK_PLUS", 
        "TK_MUL","TK_MINUS", "TK_LT", "TK_GT",
        "TK_DIV", "TK_SEMI", "$$", EPS 
    };
    G.productions["stmt-seq"] =     ProductionSet({Production(1,"stmt-seq",SymbolString({"stmt", "stmt-seqT"}))});

    G.productions["stmt-seqT"] =    ProductionSet({Production(2,"stmt-seqT",SymbolString({ "TK_SEMI", "stmt-seq"})), 
                                                   Production(3,"stmt-seqT",SymbolString({EPS}))
                                                });

    G.productions["stmt"] =         ProductionSet({Production(4,"stmt",SymbolString({"expr-stmt"})), 
                                                   Production(5,"stmt",SymbolString({"print-stmt"})),
                                                   Production(23, "stmt", SymbolString({"while-stmt"})),
                                                   Production(22, "stmt", SymbolString({EPS}))
                                                });
    G.productions["while-stmt"] =   ProductionSet({Production(24, "while-stmt", SymbolString({"TK_WHILE", "while-stmtT"}))});
    G.productions["while-stmtT"] = ProductionSet({Production(25, "while-stmtT", SymbolString({"expr", "TK_LCURLY", "stmt-seq", "TK_RCURLY"}))});
    G.productions["print-stmt"] =   ProductionSet({Production(6,"print-stmt",SymbolString({"TK_PRINT", "print-stmtT"}))});
    G.productions["print-stmtT"] = ProductionSet({Production(7,"print-stmtT",SymbolString({"expr"}))});
    G.productions["expr-stmt"] =    ProductionSet({Production(8,"expr-stmt",SymbolString({"expr"}))});
    
    G.productions["expr"] =         ProductionSet({Production(9,"expr",SymbolString({"rel-expr", "exprT"}))});
    
    G.productions["exprT"] =        ProductionSet({Production(10,"exprT",SymbolString({"TK_LT", "rel-expr", "exprT"})), 
                                                   Production(11,"exprT",SymbolString({"TK_GT", "rel-expr", "exprT"})), 
                                                   Production(12,"exprT",SymbolString({EPS}))});
    
    G.productions["rel-expr"] =         ProductionSet({Production(26,"rel-expr",SymbolString({"term", "rel-exprT"}))});
    
    G.productions["rel-exprT"] =        ProductionSet({Production(27,"rel-exprT",SymbolString({"TK_PLUS", "term", "rel-exprT"})), 
                                                   Production(28,"rel-exprT",SymbolString({"TK_MINUS", "term", "rel-exprT"})), 
                                                   Production(29,"rel-exprT",SymbolString({EPS}))});

    G.productions["term"] =         ProductionSet({Production(13,"term",SymbolString({"factor","termT"}))});
    
    G.productions["termT"] =        ProductionSet({Production(14,"termT",SymbolString({"TK_MUL", "factor", "termT"})), 
                                                   Production(15,"termT",SymbolString({"TK_DIV", "factor", "termT"})), 
                                                   Production(16,"termT",SymbolString({EPS}))});
    G.productions["factor"] =       ProductionSet({Production(17, "factor", SymbolString({"TK_MINUS", "factor"})),
                                                   Production(18, "factor", SymbolString({"primary"}))});
    G.productions["primary"] =       ProductionSet({Production(19,"primary",SymbolString({"TK_NUM"})), 
                                                   Production(20,"primary",SymbolString({"TK_ID"})), 
                                                   Production(21,"primary",SymbolString({"TK_LPAREN", "expr", "TK_RPAREN"}))});
    auto table = buildParserTable(G, "stmt-seq");
    Lexer lexer;
    StringBuffer* sb = new StringBuffer();
    sb->init(expr);
    vector<Token> tokens = lexer.lex(sb);
    AST* ast = parseInput(tokens, G, table, "stmt-seq");
    if (ast != nullptr) ast->print(), cout<<ast->eval()<<endl;
}

int main(int argc, char* argv[]) {
    expGramWithStmts(argv[1]);
    return 0;
}
