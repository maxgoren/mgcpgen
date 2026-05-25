#include <vector>
#include <map>
#include <set>
#include "production.hpp"
using namespace std; 
enum NTSYMBOL {
P, sp
};
map<int, Production> prod;
void initprod() {
	 prod[1]  = Production(1,"sp", SymbolString({"P"}),"");
	 prod[2]  = Production(2,"P", SymbolString({"TK_LPAREN","P","TK_RPAREN","P"}),"");
	 prod[3]  = Production(3,"P", SymbolString(),"");
}
map<int,map<string,string>> goTab;
void initgoTab() {
	 goTab[0] = {{"P", "1"}};
	 goTab[2] = {{"P", "3"}};
	 goTab[4] = {{"P", "5"}};
}
map<int,map<string,string>> actTab;
void initactTab() {
	 actTab[0] = {{"TK_EOI", "r3"}, {"TK_LPAREN", "s2"}, {"TK_RPAREN", "r3"}};
	 actTab[1] = {{"$", "accept"}};
	 actTab[2] = {{"TK_EOI", "r3"}, {"TK_LPAREN", "s2"}, {"TK_RPAREN", "r3"}};
	 actTab[3] = {{"TK_RPAREN", "s4"}};
	 actTab[4] = {{"TK_EOI", "r3"}, {"TK_LPAREN", "s2"}, {"TK_RPAREN", "r3"}};
	 actTab[5] = {{"TK_EOI", "r2"}, {"TK_RPAREN", "r2"}};
}
