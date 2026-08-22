#include <vector>
#include <map>
#include <set>
#include <functional>
#include "production.hpp"
using namespace std; 
enum NTSYMBOL {
AEXP, EXP, FACT, PRI, STMT, TERM, 
sp
};
map<int, Production> prod;
void initprod() {
	 prod[1]  = Production(1,"sp", SymbolString({"STMT"}),"");
	 prod[2]  = Production(2,"STMT", SymbolString({"TK_PRINT","EXP"}),"@mkPrint");
	 prod[3]  = Production(3,"STMT", SymbolString({"AEXP"}),"@through");
	 prod[4]  = Production(4,"AEXP", SymbolString({"AEXP","TK_ASSIGN","EXP"}),"@binop");
	 prod[5]  = Production(5,"AEXP", SymbolString({"EXP"}),"");
	 prod[6]  = Production(6,"EXP", SymbolString({"EXP","TK_PLUS","TERM"}),"@binop");
	 prod[7]  = Production(7,"EXP", SymbolString({"EXP","TK_MINUS","TERM"}),"@binop");
	 prod[8]  = Production(8,"EXP", SymbolString({"TERM"}),"");
	 prod[9]  = Production(9,"TERM", SymbolString({"TERM","TK_MUL","FACT"}),"@binop");
	 prod[10]  = Production(10,"TERM", SymbolString({"TERM","TK_DIV","FACT"}),"@binop");
	 prod[11]  = Production(11,"TERM", SymbolString({"FACT"}),"");
	 prod[12]  = Production(12,"FACT", SymbolString({"TK_MINUS","PRI"}),"@unary");
	 prod[13]  = Production(13,"FACT", SymbolString({"PRI"}),"");
	 prod[14]  = Production(14,"PRI", SymbolString({"TK_LPAREN","EXP","TK_RPAREN"}),"@pass");
	 prod[15]  = Production(15,"PRI", SymbolString({"TK_NUM"}),"@mkNum");
	 prod[16]  = Production(16,"PRI", SymbolString({"TK_ID"}),"@mkId");
}
map<int,map<string,string>> goTab;
void initgoTab() {
	 goTab[0] = {{"AEXP", "8"}, {"EXP", "7"}, {"FACT", "5"}, {"PRI", "3"}, {"STMT", "11"}, {"TERM", "6"}};
	 goTab[1] = {{"EXP", "12"}, {"FACT", "5"}, {"PRI", "3"}, {"TERM", "6"}};
	 goTab[4] = {{"PRI", "13"}};
	 goTab[9] = {{"EXP", "19"}, {"FACT", "5"}, {"PRI", "3"}, {"TERM", "6"}};
	 goTab[14] = {{"FACT", "21"}, {"PRI", "3"}};
	 goTab[15] = {{"FACT", "22"}, {"PRI", "3"}};
	 goTab[16] = {{"FACT", "5"}, {"PRI", "3"}, {"TERM", "23"}};
	 goTab[17] = {{"FACT", "5"}, {"PRI", "3"}, {"TERM", "24"}};
	 goTab[18] = {{"EXP", "25"}, {"FACT", "5"}, {"PRI", "3"}, {"TERM", "6"}};
}
map<int,map<string,string>> actTab;
void initactTab() {
	 actTab[0] = {{"TK_ID", "s2"}, {"TK_LPAREN", "s1"}, {"TK_MINUS", "s4"}, {"TK_NUM", "s10"}, {"TK_PRINT", "s9"}};
	 actTab[1] = {{"TK_ID", "s2"}, {"TK_LPAREN", "s1"}, {"TK_MINUS", "s4"}, {"TK_NUM", "s10"}};
	 actTab[2] = {{"TK_ASSIGN", "r16"}, {"TK_DIV", "r16"}, {"TK_EOI", "r16"}, {"TK_MINUS", "r16"}, {"TK_MUL", "r16"}, {"TK_PLUS", "r16"}, {"TK_RPAREN", "r16"}};
	 actTab[3] = {{"TK_ASSIGN", "r13"}, {"TK_DIV", "r13"}, {"TK_EOI", "r13"}, {"TK_MINUS", "r13"}, {"TK_MUL", "r13"}, {"TK_PLUS", "r13"}, {"TK_RPAREN", "r13"}};
	 actTab[4] = {{"TK_ID", "s2"}, {"TK_LPAREN", "s1"}, {"TK_NUM", "s10"}};
	 actTab[5] = {{"TK_ASSIGN", "r11"}, {"TK_DIV", "r11"}, {"TK_EOI", "r11"}, {"TK_MINUS", "r11"}, {"TK_MUL", "r11"}, {"TK_PLUS", "r11"}, {"TK_RPAREN", "r11"}};
	 actTab[6] = {{"TK_ASSIGN", "r8"}, {"TK_DIV", "s14"}, {"TK_EOI", "r8"}, {"TK_MINUS", "r8"}, {"TK_MUL", "s15"}, {"TK_PLUS", "r8"}, {"TK_RPAREN", "r8"}};
	 actTab[7] = {{"TK_ASSIGN", "r5"}, {"TK_EOI", "r5"}, {"TK_MINUS", "s16"}, {"TK_PLUS", "s17"}};
	 actTab[8] = {{"TK_ASSIGN", "s18"}, {"TK_EOI", "r3"}};
	 actTab[9] = {{"TK_ID", "s2"}, {"TK_LPAREN", "s1"}, {"TK_MINUS", "s4"}, {"TK_NUM", "s10"}};
	 actTab[10] = {{"TK_ASSIGN", "r15"}, {"TK_DIV", "r15"}, {"TK_EOI", "r15"}, {"TK_MINUS", "r15"}, {"TK_MUL", "r15"}, {"TK_PLUS", "r15"}, {"TK_RPAREN", "r15"}};
	 actTab[11] = {{"$", "accept"}};
	 actTab[12] = {{"TK_MINUS", "s16"}, {"TK_PLUS", "s17"}, {"TK_RPAREN", "s20"}};
	 actTab[13] = {{"TK_ASSIGN", "r12"}, {"TK_DIV", "r12"}, {"TK_EOI", "r12"}, {"TK_MINUS", "r12"}, {"TK_MUL", "r12"}, {"TK_PLUS", "r12"}, {"TK_RPAREN", "r12"}};
	 actTab[14] = {{"TK_ID", "s2"}, {"TK_LPAREN", "s1"}, {"TK_MINUS", "s4"}, {"TK_NUM", "s10"}};
	 actTab[15] = {{"TK_ID", "s2"}, {"TK_LPAREN", "s1"}, {"TK_MINUS", "s4"}, {"TK_NUM", "s10"}};
	 actTab[16] = {{"TK_ID", "s2"}, {"TK_LPAREN", "s1"}, {"TK_MINUS", "s4"}, {"TK_NUM", "s10"}};
	 actTab[17] = {{"TK_ID", "s2"}, {"TK_LPAREN", "s1"}, {"TK_MINUS", "s4"}, {"TK_NUM", "s10"}};
	 actTab[18] = {{"TK_ID", "s2"}, {"TK_LPAREN", "s1"}, {"TK_MINUS", "s4"}, {"TK_NUM", "s10"}};
	 actTab[19] = {{"TK_EOI", "r2"}, {"TK_MINUS", "s16"}, {"TK_PLUS", "s17"}};
	 actTab[20] = {{"TK_ASSIGN", "r14"}, {"TK_DIV", "r14"}, {"TK_EOI", "r14"}, {"TK_MINUS", "r14"}, {"TK_MUL", "r14"}, {"TK_PLUS", "r14"}, {"TK_RPAREN", "r14"}};
	 actTab[21] = {{"TK_ASSIGN", "r10"}, {"TK_DIV", "r10"}, {"TK_EOI", "r10"}, {"TK_MINUS", "r10"}, {"TK_MUL", "r10"}, {"TK_PLUS", "r10"}, {"TK_RPAREN", "r10"}};
	 actTab[22] = {{"TK_ASSIGN", "r9"}, {"TK_DIV", "r9"}, {"TK_EOI", "r9"}, {"TK_MINUS", "r9"}, {"TK_MUL", "r9"}, {"TK_PLUS", "r9"}, {"TK_RPAREN", "r9"}};
	 actTab[23] = {{"TK_ASSIGN", "r7"}, {"TK_DIV", "s14"}, {"TK_EOI", "r7"}, {"TK_MINUS", "r7"}, {"TK_MUL", "s15"}, {"TK_PLUS", "r7"}, {"TK_RPAREN", "r7"}};
	 actTab[24] = {{"TK_ASSIGN", "r6"}, {"TK_DIV", "s14"}, {"TK_EOI", "r6"}, {"TK_MINUS", "r6"}, {"TK_MUL", "s15"}, {"TK_PLUS", "r6"}, {"TK_RPAREN", "r6"}};
	 actTab[25] = {{"TK_ASSIGN", "r4"}, {"TK_EOI", "r4"}, {"TK_MINUS", "s16"}, {"TK_PLUS", "s17"}};
}
map<string, function<AST*(vector<AST*>&)>> actions;
void initActions() {
	 actions.insert({"binop",binop});
	 actions.insert({"mkId",mkId});
	 actions.insert({"mkNum",mkNum});
	 actions.insert({"mkPrint",mkPrint});
	 actions.insert({"pass",pass});
	 actions.insert({"through",through});
	 actions.insert({"unary",unary});
}
