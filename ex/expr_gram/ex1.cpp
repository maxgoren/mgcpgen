#include "../src/cfg.hpp"
#include "../src/calc_firsts.hpp"
#include "../src/calc_follows.hpp"
#include "../src/build_table.hpp"

//LL(1) Expression Grammar - Taken from Louden - Compiler Construction Principles and Practice
void expGram() {
    Grammar B;
    B.nonterminals = { "exp", "expT", "addop", "term", "termT", "mulop", "factor"};
    B.terminals = { "TK_ID", "TK_NUM", "TK_LPAREN", "TK_RPAREN", "TK_PLUS", "TK_MUL", "TK_MINUS","TK_DIV", EPS, GOAL };
    B.productions["exp"] = ProductionSet({Production(1,"exp", SymbolString({"term", "expT"}))});
    B.productions["expT"] = ProductionSet({Production(2,"expT", SymbolString({"addop", "term", "expT"})), Production(3,"expT",SymbolString({EPS}))});
    B.productions["term"] = ProductionSet({Production(4,"term", SymbolString({"factor", "termT"}))});
    B.productions["termT"] = ProductionSet({Production(5,"termT", SymbolString({ "mulop", "factor", "termT"})), Production(6,"termT",SymbolString({EPS}))});
    B.productions["factor"] = ProductionSet({Production(7,"factor",SymbolString({"TK_ID"})), Production(8,"factor",SymbolString({"TK_NUM"})), Production(9,"factor",SymbolString({"TK_LPAREN", "exp", "TK_RPAREN"}))});
    B.productions["addop"]  =ProductionSet({Production(10,"addop",SymbolString({"TK_PLUS"})), Production(11,"addop",SymbolString({"TK_MINUS"}))});
    B.productions["mulop"] = ProductionSet({Production(12,"mulop",SymbolString({"TK_MUL"})), Production(13,"mulop",SymbolString({"TK_DIV"}))});
    calcFirstSets(B);
    calcFollowSets(B, "exp");
    printFirsts(B);
    printFollows(B);
}

int main(int argc, char* argv[]) {
    expGram();
    return 0;
}
