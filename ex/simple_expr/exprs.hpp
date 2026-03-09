#include <vector>
#include <map>
#include <set>
#include "cfg.hpp"
using namespace std; 
set<Symbol> terminalSymbols = {"#", "TK_ASSIGN", "TK_DIV", "TK_EOI", "TK_GT", "TK_ID", "TK_LPAREN", "TK_LT", "TK_MINUS", "TK_MUL", "TK_NUM", "TK_PLUS", "TK_RPAREN"};
set<Symbol> nonTerminalSymbols = {"#", "assign-expr", "assign-exprT", "expr", "exprT", "factor", "primary", "rel-expr", "rel-exprT", "stmt", "term", "termT"};
typedef map<Symbol, map<Symbol, Production>> ParseTable;
ParseTable parseTable;
void initParseTable() {
	 parseTable["assign-expr"]["TK_ID"] = Production(5,"assign-expr", SymbolString({"rel-expr","assign-exprT"}));
	 parseTable["assign-expr"]["TK_LPAREN"] = Production(5,"assign-expr", SymbolString({"rel-expr","assign-exprT"}));
	 parseTable["assign-expr"]["TK_MINUS"] = Production(5,"assign-expr", SymbolString({"rel-expr","assign-exprT"}));
	 parseTable["assign-expr"]["TK_NUM"] = Production(5,"assign-expr", SymbolString({"rel-expr","assign-exprT"}));
	 parseTable["assign-exprT"]["TK_ASSIGN"] = Production(8,"assign-exprT", SymbolString({"#"}));
	 parseTable["assign-exprT"]["TK_EOI"] = Production(8,"assign-exprT", SymbolString({"#"}));
	 parseTable["assign-exprT"]["TK_GT"] = Production(7,"assign-exprT", SymbolString({"TK_GT","rel-expr","@@","assign-exprT"}));
	 parseTable["assign-exprT"]["TK_LT"] = Production(6,"assign-exprT", SymbolString({"TK_LT","rel-expr","@@","assign-exprT"}));
	 parseTable["assign-exprT"]["TK_RPAREN"] = Production(8,"assign-exprT", SymbolString({"#"}));
	 parseTable["expr"]["TK_ID"] = Production(2,"expr", SymbolString({"assign-expr","exprT"}));
	 parseTable["expr"]["TK_LPAREN"] = Production(2,"expr", SymbolString({"assign-expr","exprT"}));
	 parseTable["expr"]["TK_MINUS"] = Production(2,"expr", SymbolString({"assign-expr","exprT"}));
	 parseTable["expr"]["TK_NUM"] = Production(2,"expr", SymbolString({"assign-expr","exprT"}));
	 parseTable["exprT"]["TK_ASSIGN"] = Production(3,"exprT", SymbolString({"TK_ASSIGN","assign-expr","@@","exprT"}));
	 parseTable["exprT"]["TK_EOI"] = Production(4,"exprT", SymbolString({"#"}));
	 parseTable["exprT"]["TK_RPAREN"] = Production(4,"exprT", SymbolString({"#"}));
	 parseTable["factor"]["TK_ID"] = Production(18,"factor", SymbolString({"primary"}));
	 parseTable["factor"]["TK_LPAREN"] = Production(18,"factor", SymbolString({"primary"}));
	 parseTable["factor"]["TK_MINUS"] = Production(17,"factor", SymbolString({"TK_MINUS","factor","@@"}));
	 parseTable["factor"]["TK_NUM"] = Production(18,"factor", SymbolString({"primary"}));
	 parseTable["primary"]["TK_ID"] = Production(19,"primary", SymbolString({"TK_ID","@@"}));
	 parseTable["primary"]["TK_LPAREN"] = Production(21,"primary", SymbolString({"TK_LPAREN","expr","TK_RPAREN","@@"}));
	 parseTable["primary"]["TK_NUM"] = Production(20,"primary", SymbolString({"TK_NUM","@@"}));
	 parseTable["rel-expr"]["TK_ID"] = Production(9,"rel-expr", SymbolString({"term","rel-exprT"}));
	 parseTable["rel-expr"]["TK_LPAREN"] = Production(9,"rel-expr", SymbolString({"term","rel-exprT"}));
	 parseTable["rel-expr"]["TK_MINUS"] = Production(9,"rel-expr", SymbolString({"term","rel-exprT"}));
	 parseTable["rel-expr"]["TK_NUM"] = Production(9,"rel-expr", SymbolString({"term","rel-exprT"}));
	 parseTable["rel-exprT"]["TK_ASSIGN"] = Production(12,"rel-exprT", SymbolString({"#"}));
	 parseTable["rel-exprT"]["TK_EOI"] = Production(12,"rel-exprT", SymbolString({"#"}));
	 parseTable["rel-exprT"]["TK_GT"] = Production(12,"rel-exprT", SymbolString({"#"}));
	 parseTable["rel-exprT"]["TK_LT"] = Production(12,"rel-exprT", SymbolString({"#"}));
	 parseTable["rel-exprT"]["TK_MINUS"] = Production(11,"rel-exprT", SymbolString({"TK_MINUS","term","@@","rel-exprT"}));
	 parseTable["rel-exprT"]["TK_PLUS"] = Production(10,"rel-exprT", SymbolString({"TK_PLUS","term","@@","rel-exprT"}));
	 parseTable["rel-exprT"]["TK_RPAREN"] = Production(12,"rel-exprT", SymbolString({"#"}));
	 parseTable["stmt"]["TK_ID"] = Production(1,"stmt", SymbolString({"expr"}));
	 parseTable["stmt"]["TK_LPAREN"] = Production(1,"stmt", SymbolString({"expr"}));
	 parseTable["stmt"]["TK_MINUS"] = Production(1,"stmt", SymbolString({"expr"}));
	 parseTable["stmt"]["TK_NUM"] = Production(1,"stmt", SymbolString({"expr"}));
	 parseTable["term"]["TK_ID"] = Production(13,"term", SymbolString({"factor","termT"}));
	 parseTable["term"]["TK_LPAREN"] = Production(13,"term", SymbolString({"factor","termT"}));
	 parseTable["term"]["TK_MINUS"] = Production(13,"term", SymbolString({"factor","termT"}));
	 parseTable["term"]["TK_NUM"] = Production(13,"term", SymbolString({"factor","termT"}));
	 parseTable["termT"]["TK_ASSIGN"] = Production(16,"termT", SymbolString({"#"}));
	 parseTable["termT"]["TK_DIV"] = Production(15,"termT", SymbolString({"TK_DIV","factor","@@","termT"}));
	 parseTable["termT"]["TK_EOI"] = Production(16,"termT", SymbolString({"#"}));
	 parseTable["termT"]["TK_GT"] = Production(16,"termT", SymbolString({"#"}));
	 parseTable["termT"]["TK_LT"] = Production(16,"termT", SymbolString({"#"}));
	 parseTable["termT"]["TK_MINUS"] = Production(16,"termT", SymbolString({"#"}));
	 parseTable["termT"]["TK_MUL"] = Production(14,"termT", SymbolString({"TK_MUL","factor","@@","termT"}));
	 parseTable["termT"]["TK_PLUS"] = Production(16,"termT", SymbolString({"#"}));
	 parseTable["termT"]["TK_RPAREN"] = Production(16,"termT", SymbolString({"#"}));
}
