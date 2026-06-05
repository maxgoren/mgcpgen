#include <vector>
#include <map>
#include <set>
#include "production.hpp"
using namespace std; 
enum NTSYMBOL {
EXP, 
STMT, sp
};
map<int, Production> prod;
void initprod() {
	 prod[1]  = Production(1,"sp", SymbolString({"STMT"}),"");
	 prod[2]  = Production(2,"STMT", SymbolString({"TK_PRINT","EXP"}),"@mkprint");
	 prod[3]  = Production(3,"STMT", SymbolString({"EXP"}),"@mkexprstmt");
	 prod[4]  = Production(4,"EXP", SymbolString({"EXP","TK_PLUS","EXP"}),"@binop");
	 prod[5]  = Production(5,"EXP", SymbolString({"EXP","TK_MINUS","EXP"}),"@binop");
	 prod[6]  = Production(6,"EXP", SymbolString({"EXP","TK_MUL","EXP"}),"@binop");
	 prod[7]  = Production(7,"EXP", SymbolString({"EXP","TK_DIV","EXP"}),"@binop");
	 prod[8]  = Production(8,"EXP", SymbolString({"TK_LPAREN","EXP","TK_RPAREN"}),"@pass");
	 prod[9]  = Production(9,"EXP", SymbolString({"TK_MINUS","EXP"}),"@unary");
	 prod[10]  = Production(10,"EXP", SymbolString({"TK_NUM"}),"@num");
}
map<int,map<string,string>> goTab;
void initgoTab() {
	 goTab[0] = {{"EXP", "2"}, {"STMT", "5"}};
	 goTab[1] = {{"EXP", "7"}};
	 goTab[3] = {{"EXP", "12"}};
	 goTab[4] = {{"EXP", "13"}};
	 goTab[8] = {{"EXP", "14"}};
	 goTab[9] = {{"EXP", "15"}};
	 goTab[10] = {{"EXP", "16"}};
	 goTab[11] = {{"EXP", "17"}};
}
map<int,map<string,string>> actTab;
void initactTab() {
	 actTab[0] = {{"TK_LPAREN", "s3"}, {"TK_MINUS", "s4"}, {"TK_NUM", "s6"}, {"TK_PRINT", "s1"}};
	 actTab[1] = {{"TK_LPAREN", "s3"}, {"TK_MINUS", "s4"}, {"TK_NUM", "s6"}};
	 actTab[2] = {{"TK_DIV", "s11"}, {"TK_EOI", "r3"}, {"TK_MINUS", "s8"}, {"TK_MUL", "s9"}, {"TK_PLUS", "s10"}};
	 actTab[3] = {{"TK_LPAREN", "s3"}, {"TK_MINUS", "s4"}, {"TK_NUM", "s6"}};
	 actTab[4] = {{"TK_LPAREN", "s3"}, {"TK_MINUS", "s4"}, {"TK_NUM", "s6"}};
	 actTab[5] = {{"$", "accept"}};
	 actTab[6] = {{"TK_DIV", "r10"}, {"TK_EOI", "r10"}, {"TK_MINUS", "r10"}, {"TK_MUL", "r10"}, {"TK_PLUS", "r10"}, {"TK_RPAREN", "r10"}};
	 actTab[7] = {{"TK_DIV", "s11"}, {"TK_EOI", "r2"}, {"TK_MINUS", "s8"}, {"TK_MUL", "s9"}, {"TK_PLUS", "s10"}};
	 actTab[8] = {{"TK_LPAREN", "s3"}, {"TK_MINUS", "s4"}, {"TK_NUM", "s6"}};
	 actTab[9] = {{"TK_LPAREN", "s3"}, {"TK_MINUS", "s4"}, {"TK_NUM", "s6"}};
	 actTab[10] = {{"TK_LPAREN", "s3"}, {"TK_MINUS", "s4"}, {"TK_NUM", "s6"}};
	 actTab[11] = {{"TK_LPAREN", "s3"}, {"TK_MINUS", "s4"}, {"TK_NUM", "s6"}};
	 actTab[12] = {{"TK_DIV", "s11"}, {"TK_MINUS", "s8"}, {"TK_MUL", "s9"}, {"TK_PLUS", "s10"}, {"TK_RPAREN", "s18"}};
	 actTab[13] = {{"TK_DIV", "s11"}, {"TK_EOI", "r9"}, {"TK_MINUS", "r9"}, {"TK_MUL", "s9"}, {"TK_PLUS", "r9"}, {"TK_RPAREN", "r9"}};
	 actTab[14] = {{"TK_DIV", "s11"}, {"TK_EOI", "r5"}, {"TK_MINUS", "r5"}, {"TK_MUL", "s9"}, {"TK_PLUS", "r5"}, {"TK_RPAREN", "r5"}};
	 actTab[15] = {{"TK_DIV", "r6"}, {"TK_EOI", "r6"}, {"TK_MINUS", "r6"}, {"TK_MUL", "r6"}, {"TK_PLUS", "r6"}, {"TK_RPAREN", "r6"}};
	 actTab[16] = {{"TK_DIV", "s11"}, {"TK_EOI", "r4"}, {"TK_MINUS", "r4"}, {"TK_MUL", "s9"}, {"TK_PLUS", "r4"}, {"TK_RPAREN", "r4"}};
	 actTab[17] = {{"TK_DIV", "r7"}, {"TK_EOI", "r7"}, {"TK_MINUS", "r7"}, {"TK_MUL", "r7"}, {"TK_PLUS", "r7"}, {"TK_RPAREN", "r7"}};
	 actTab[18] = {{"TK_DIV", "r8"}, {"TK_EOI", "r8"}, {"TK_MINUS", "r8"}, {"TK_MUL", "r8"}, {"TK_PLUS", "r8"}, {"TK_RPAREN", "r8"}};
}
