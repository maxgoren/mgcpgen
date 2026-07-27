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
	 goTab[3] = {{"EXP", "13"}};
	 goTab[4] = {{"EXP", "16"}};
	 goTab[8] = {{"EXP", "17"}};
	 goTab[9] = {{"EXP", "18"}};
	 goTab[10] = {{"EXP", "19"}};
	 goTab[11] = {{"EXP", "20"}};
	 goTab[14] = {{"EXP", "26"}};
	 goTab[15] = {{"EXP", "27"}};
	 goTab[22] = {{"EXP", "28"}};
	 goTab[23] = {{"EXP", "29"}};
	 goTab[24] = {{"EXP", "30"}};
	 goTab[25] = {{"EXP", "31"}};
}
map<int,map<string,string>> actTab;
void initactTab() {
	 actTab[0] = {{"TK_LPAREN", "s3"}, {"TK_MINUS", "s4"}, {"TK_NUM", "s6"}, {"TK_PRINT", "s1"}};
	 actTab[1] = {{"TK_LPAREN", "s3"}, {"TK_MINUS", "s4"}, {"TK_NUM", "s6"}};
	 actTab[2] = {{"TK_DIV", "s11"}, {"TK_MINUS", "s8"}, {"TK_MUL", "s9"}, {"TK_PLUS", "s10"}};
	 actTab[3] = {{"TK_LPAREN", "s14"}, {"TK_MINUS", "s15"}, {"TK_NUM", "s12"}};
	 actTab[4] = {{"TK_LPAREN", "s3"}, {"TK_MINUS", "s4"}, {"TK_NUM", "s6"}};
	 actTab[7] = {{"TK_DIV", "s11"}, {"TK_MINUS", "s8"}, {"TK_MUL", "s9"}, {"TK_PLUS", "s10"}};
	 actTab[8] = {{"TK_LPAREN", "s3"}, {"TK_MINUS", "s4"}, {"TK_NUM", "s6"}};
	 actTab[9] = {{"TK_LPAREN", "s3"}, {"TK_MINUS", "s4"}, {"TK_NUM", "s6"}};
	 actTab[10] = {{"TK_LPAREN", "s3"}, {"TK_MINUS", "s4"}, {"TK_NUM", "s6"}};
	 actTab[11] = {{"TK_LPAREN", "s3"}, {"TK_MINUS", "s4"}, {"TK_NUM", "s6"}};
	 actTab[12] = {{"TK_RPAREN", "r10"}};
	 actTab[13] = {{"TK_DIV", "s25"}, {"TK_MINUS", "s22"}, {"TK_MUL", "s23"}, {"TK_PLUS", "s24"}, {"TK_RPAREN", "s21"}};
	 actTab[14] = {{"TK_LPAREN", "s14"}, {"TK_MINUS", "s15"}, {"TK_NUM", "s12"}};
	 actTab[15] = {{"TK_LPAREN", "s14"}, {"TK_MINUS", "s15"}, {"TK_NUM", "s12"}};
	 actTab[16] = {{"TK_DIV", "s11"}, {"TK_MINUS", "s8"}, {"TK_MUL", "s9"}, {"TK_PLUS", "s10"}};
	 actTab[17] = {{"TK_DIV", "s11"}, {"TK_MINUS", "s8"}, {"TK_MUL", "s9"}, {"TK_PLUS", "s10"}};
	 actTab[18] = {{"TK_DIV", "s11"}, {"TK_MINUS", "s8"}, {"TK_MUL", "s9"}, {"TK_PLUS", "s10"}};
	 actTab[19] = {{"TK_DIV", "s11"}, {"TK_MINUS", "s8"}, {"TK_MUL", "s9"}, {"TK_PLUS", "s10"}};
	 actTab[20] = {{"TK_DIV", "s11"}, {"TK_MINUS", "s8"}, {"TK_MUL", "s9"}, {"TK_PLUS", "s10"}};
	 actTab[22] = {{"TK_LPAREN", "s14"}, {"TK_MINUS", "s15"}, {"TK_NUM", "s12"}};
	 actTab[23] = {{"TK_LPAREN", "s14"}, {"TK_MINUS", "s15"}, {"TK_NUM", "s12"}};
	 actTab[24] = {{"TK_LPAREN", "s14"}, {"TK_MINUS", "s15"}, {"TK_NUM", "s12"}};
	 actTab[25] = {{"TK_LPAREN", "s14"}, {"TK_MINUS", "s15"}, {"TK_NUM", "s12"}};
	 actTab[26] = {{"TK_DIV", "s25"}, {"TK_MINUS", "s22"}, {"TK_MUL", "s23"}, {"TK_PLUS", "s24"}, {"TK_RPAREN", "s32"}};
	 actTab[27] = {{"TK_DIV", "s25"}, {"TK_MINUS", "s22"}, {"TK_MUL", "s23"}, {"TK_PLUS", "s24"}, {"TK_RPAREN", "r9"}};
	 actTab[28] = {{"TK_DIV", "s25"}, {"TK_MINUS", "s22"}, {"TK_MUL", "s23"}, {"TK_PLUS", "s24"}, {"TK_RPAREN", "r5"}};
	 actTab[29] = {{"TK_DIV", "s25"}, {"TK_MINUS", "s22"}, {"TK_MUL", "s23"}, {"TK_PLUS", "s24"}, {"TK_RPAREN", "r6"}};
	 actTab[30] = {{"TK_DIV", "s25"}, {"TK_MINUS", "s22"}, {"TK_MUL", "s23"}, {"TK_PLUS", "s24"}, {"TK_RPAREN", "r4"}};
	 actTab[31] = {{"TK_DIV", "s25"}, {"TK_MINUS", "s22"}, {"TK_MUL", "s23"}, {"TK_PLUS", "s24"}, {"TK_RPAREN", "r7"}};
	 actTab[32] = {{"TK_RPAREN", "r8"}};
}
