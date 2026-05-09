#include <vector>
#include <map>
#include <set>
#include "cfg.hpp"
using namespace std; 
set<Symbol> terminalSymbols = {"#", "TK_EOI", "TK_ID", "TK_LPAREN", "TK_NUMBER", "TK_OP", "TK_QUOTE", "TK_RPAREN"};
set<Symbol> nonTerminalSymbols = {"#", "atom", "elmnts", "list", "sexpr"};
typedef map<Symbol, map<Symbol, Production>> ParseTable;
ParseTable parseTable;
void initParseTable() {
	 parseTable["atom"]["TK_ID"] = Production(6,"atom", SymbolString({"TK_ID"}));
	 parseTable["atom"]["TK_NUMBER"] = Production(7,"atom", SymbolString({"TK_NUMBER"}));
	 parseTable["atom"]["TK_OP"] = Production(9,"atom", SymbolString({"TK_OP"}));
	 parseTable["atom"]["TK_QUOTE"] = Production(8,"atom", SymbolString({"TK_QUOTE","atom"}));
	 parseTable["elmnts"]["TK_ID"] = Production(4,"elmnts", SymbolString({"sexpr","elmnts"}));
	 parseTable["elmnts"]["TK_LPAREN"] = Production(4,"elmnts", SymbolString({"sexpr","elmnts"}));
	 parseTable["elmnts"]["TK_NUMBER"] = Production(4,"elmnts", SymbolString({"sexpr","elmnts"}));
	 parseTable["elmnts"]["TK_OP"] = Production(4,"elmnts", SymbolString({"sexpr","elmnts"}));
	 parseTable["elmnts"]["TK_QUOTE"] = Production(4,"elmnts", SymbolString({"sexpr","elmnts"}));
	 parseTable["elmnts"]["TK_RPAREN"] = Production(5,"elmnts", SymbolString({"#"}));
	 parseTable["list"]["TK_LPAREN"] = Production(3,"list", SymbolString({"TK_LPAREN","elmnts","TK_RPAREN","@@"}));
	 parseTable["sexpr"]["TK_ID"] = Production(1,"sexpr", SymbolString({"atom"}));
	 parseTable["sexpr"]["TK_LPAREN"] = Production(2,"sexpr", SymbolString({"list"}));
	 parseTable["sexpr"]["TK_NUMBER"] = Production(1,"sexpr", SymbolString({"atom"}));
	 parseTable["sexpr"]["TK_OP"] = Production(1,"sexpr", SymbolString({"atom"}));
	 parseTable["sexpr"]["TK_QUOTE"] = Production(1,"sexpr", SymbolString({"atom"}));
}
