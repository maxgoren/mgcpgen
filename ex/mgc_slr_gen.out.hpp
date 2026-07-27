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
	 goTab[0] = {{"EXP", "6"}, {"STMT", "3"}};
	 goTab[1] = {{"EXP", "8"}};
	 goTab[2] = {{"EXP", "11"}};
	 goTab[5] = {{"EXP", "12"}};
	 goTab[9] = {{"EXP", "22"}};
	 goTab[10] = {{"EXP", "23"}};
	 goTab[13] = {{"EXP", "24"}};
	 goTab[14] = {{"EXP", "25"}};
	 goTab[15] = {{"EXP", "26"}};
	 goTab[16] = {{"EXP", "27"}};
	 goTab[17] = {{"EXP", "28"}};
	 goTab[19] = {{"EXP", "29"}};
	 goTab[20] = {{"EXP", "30"}};
	 goTab[21] = {{"EXP", "31"}};
}
map<int,map<string,string>> actTab;
void initactTab() {
	 actTab[0] = {{"TK_LPAREN", "s1"}, {"TK_MINUS", "s5"}, {"TK_NUM", "s4"}, {"TK_PRINT", "s2"}};
	 actTab[1] = {{"TK_LPAREN", "s9"}, {"TK_MINUS", "s10"}, {"TK_NUM", "s7"}};
	 actTab[2] = {{"TK_LPAREN", "s1"}, {"TK_MINUS", "s5"}, {"TK_NUM", "s4"}};
	 actTab[4] = {{"TK_DIV", "r10"}, {"TK_LPAREN", "r10"}, {"TK_MINUS", "r10"}, {"TK_MUL", "r10"}, {"TK_NUM", "r10"}, {"TK_PLUS", "r10"}};
	 actTab[5] = {{"TK_LPAREN", "s1"}, {"TK_MINUS", "s5"}, {"TK_NUM", "s4"}};
	 actTab[6] = {{"TK_DIV", "s16"}, {"TK_MINUS", "s13"}, {"TK_MUL", "s14"}, {"TK_PLUS", "s15"}};
	 actTab[7] = {{"TK_DIV", "r10"}, {"TK_LPAREN", "r10"}, {"TK_MINUS", "r10"}, {"TK_MUL", "r10"}, {"TK_NUM", "r10"}, {"TK_PLUS", "r10"}, {"TK_RPAREN", "r10"}};
	 actTab[8] = {{"TK_DIV", "s19"}, {"TK_MINUS", "s21"}, {"TK_MUL", "s17"}, {"TK_PLUS", "s20"}, {"TK_RPAREN", "s18"}};
	 actTab[9] = {{"TK_LPAREN", "s9"}, {"TK_MINUS", "s10"}, {"TK_NUM", "s7"}};
	 actTab[10] = {{"TK_LPAREN", "s9"}, {"TK_MINUS", "s10"}, {"TK_NUM", "s7"}};
	 actTab[11] = {{"TK_DIV", "s16"}, {"TK_MINUS", "s13"}, {"TK_MUL", "s14"}, {"TK_PLUS", "s15"}};
	 actTab[12] = {{"TK_DIV", "r9"}, {"TK_LPAREN", "r9"}, {"TK_MINUS", "r9"}, {"TK_MUL", "r9"}, {"TK_NUM", "r9"}, {"TK_PLUS", "r9"}};
	 actTab[13] = {{"TK_LPAREN", "s1"}, {"TK_MINUS", "s5"}, {"TK_NUM", "s4"}};
	 actTab[14] = {{"TK_LPAREN", "s1"}, {"TK_MINUS", "s5"}, {"TK_NUM", "s4"}};
	 actTab[15] = {{"TK_LPAREN", "s1"}, {"TK_MINUS", "s5"}, {"TK_NUM", "s4"}};
	 actTab[16] = {{"TK_LPAREN", "s1"}, {"TK_MINUS", "s5"}, {"TK_NUM", "s4"}};
	 actTab[17] = {{"TK_LPAREN", "s9"}, {"TK_MINUS", "s10"}, {"TK_NUM", "s7"}};
	 actTab[18] = {{"TK_DIV", "r8"}, {"TK_LPAREN", "r8"}, {"TK_MINUS", "r8"}, {"TK_MUL", "r8"}, {"TK_NUM", "r8"}, {"TK_PLUS", "r8"}};
	 actTab[19] = {{"TK_LPAREN", "s9"}, {"TK_MINUS", "s10"}, {"TK_NUM", "s7"}};
	 actTab[20] = {{"TK_LPAREN", "s9"}, {"TK_MINUS", "s10"}, {"TK_NUM", "s7"}};
	 actTab[21] = {{"TK_LPAREN", "s9"}, {"TK_MINUS", "s10"}, {"TK_NUM", "s7"}};
	 actTab[22] = {{"TK_DIV", "s19"}, {"TK_MINUS", "s21"}, {"TK_MUL", "s17"}, {"TK_PLUS", "s20"}, {"TK_RPAREN", "s32"}};
	 actTab[23] = {{"TK_DIV", "r9"}, {"TK_LPAREN", "r9"}, {"TK_MINUS", "r9"}, {"TK_MUL", "r9"}, {"TK_NUM", "r9"}, {"TK_PLUS", "r9"}, {"TK_RPAREN", "r9"}};
	 actTab[24] = {{"TK_DIV", "s16"}, {"TK_LPAREN", "r5"}, {"TK_MINUS", "r5"}, {"TK_MUL", "s14"}, {"TK_NUM", "r5"}, {"TK_PLUS", "r5"}};
	 actTab[25] = {{"TK_DIV", "r6"}, {"TK_LPAREN", "r6"}, {"TK_MINUS", "r6"}, {"TK_MUL", "r6"}, {"TK_NUM", "r6"}, {"TK_PLUS", "r6"}};
	 actTab[26] = {{"TK_DIV", "s16"}, {"TK_LPAREN", "r4"}, {"TK_MINUS", "r4"}, {"TK_MUL", "s14"}, {"TK_NUM", "r4"}, {"TK_PLUS", "r4"}};
	 actTab[27] = {{"TK_DIV", "r7"}, {"TK_LPAREN", "r7"}, {"TK_MINUS", "r7"}, {"TK_MUL", "r7"}, {"TK_NUM", "r7"}, {"TK_PLUS", "r7"}};
	 actTab[28] = {{"TK_DIV", "r6"}, {"TK_LPAREN", "r6"}, {"TK_MINUS", "r6"}, {"TK_MUL", "r6"}, {"TK_NUM", "r6"}, {"TK_PLUS", "r6"}, {"TK_RPAREN", "r6"}};
	 actTab[29] = {{"TK_DIV", "r7"}, {"TK_LPAREN", "r7"}, {"TK_MINUS", "r7"}, {"TK_MUL", "r7"}, {"TK_NUM", "r7"}, {"TK_PLUS", "r7"}, {"TK_RPAREN", "r7"}};
	 actTab[30] = {{"TK_DIV", "s19"}, {"TK_LPAREN", "r4"}, {"TK_MINUS", "r4"}, {"TK_MUL", "s17"}, {"TK_NUM", "r4"}, {"TK_PLUS", "r4"}, {"TK_RPAREN", "r4"}};
	 actTab[31] = {{"TK_DIV", "s19"}, {"TK_LPAREN", "r5"}, {"TK_MINUS", "r5"}, {"TK_MUL", "s17"}, {"TK_NUM", "r5"}, {"TK_PLUS", "r5"}, {"TK_RPAREN", "r5"}};
	 actTab[32] = {{"TK_DIV", "r8"}, {"TK_LPAREN", "r8"}, {"TK_MINUS", "r8"}, {"TK_MUL", "r8"}, {"TK_NUM", "r8"}, {"TK_PLUS", "r8"}, {"TK_RPAREN", "r8"}};
}
