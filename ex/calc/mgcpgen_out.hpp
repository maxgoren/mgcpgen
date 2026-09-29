#include <vector>
#include <map>
#include <set>
#include <functional>
#include "production.hpp"
using namespace std; 
enum NTSYMBOL {
ADDOP, AEXP, EXP, FACT, MULOP, PRI, 
STMT, TERM, sp
};
static const Production prod[] = {
 	 Production(0, "dummy", SymbolString(), ""),

	 Production(1,"sp", SymbolString({"STMT"}),""), 
	 Production(2,"STMT", SymbolString({"TK_PRINT","EXP"}),"@mkPrint"), 
	 Production(3,"STMT", SymbolString({"AEXP"}),""), 
	 Production(4,"AEXP", SymbolString({"AEXP","TK_ASSIGN","EXP"}),"@binop"), 
	 Production(5,"AEXP", SymbolString({"EXP"}),""), 
	 Production(6,"EXP", SymbolString({"EXP","ADDOP","TERM"}),"@binop"), 
	 Production(7,"EXP", SymbolString({"TERM"}),""), 
	 Production(8,"TERM", SymbolString({"TERM","MULOP","FACT"}),"@binop"), 
	 Production(9,"TERM", SymbolString({"FACT"}),""), 
	 Production(10,"FACT", SymbolString({"TK_MINUS","PRI"}),"@unary"), 
	 Production(11,"FACT", SymbolString({"PRI"}),""), 
	 Production(12,"PRI", SymbolString({"TK_LPAREN","EXP","TK_RPAREN"}),"@pass"), 
	 Production(13,"PRI", SymbolString({"TK_NUM"}),"@mkNum"), 
	 Production(14,"PRI", SymbolString({"TK_ID"}),"@mkId"), 
	 Production(15,"ADDOP", SymbolString({"TK_PLUS"}),""), 
	 Production(16,"ADDOP", SymbolString({"TK_MINUS"}),""), 
	 Production(17,"MULOP", SymbolString({"TK_MUL"}),""), 
	 Production(18,"MULOP", SymbolString({"TK_DIV"}),"")
};
static const string goTab_row_0[] = {"6","AEXP", "8", "EXP", "7", "FACT", "5", "PRI", "3", "STMT", "11", "TERM", "6"};
static const string goTab_row_1[] = {"4","EXP", "12", "FACT", "5", "PRI", "3", "TERM", "6"};
static const string goTab_row_4[] = {"1","PRI", "13"};
static const string goTab_row_6[] = {"1","MULOP", "16"};
static const string goTab_row_7[] = {"1","ADDOP", "19"};
static const string goTab_row_9[] = {"4","EXP", "21", "FACT", "5", "PRI", "3", "TERM", "6"};
static const string goTab_row_12[] = {"1","ADDOP", "19"};
static const string goTab_row_16[] = {"2","FACT", "23", "PRI", "3"};
static const string goTab_row_19[] = {"3","FACT", "5", "PRI", "3", "TERM", "24"};
static const string goTab_row_20[] = {"4","EXP", "25", "FACT", "5", "PRI", "3", "TERM", "6"};
static const string goTab_row_21[] = {"1","ADDOP", "19"};
static const string goTab_row_24[] = {"1","MULOP", "16"};
static const string goTab_row_25[] = {"1","ADDOP", "19"};
static const string *goTab[] = {
	 goTab_row_0, 	 goTab_row_1, 	 NULL, 	 NULL, 	 goTab_row_4, 
	 NULL, 	 goTab_row_6, 	 goTab_row_7, 	 NULL, 	 goTab_row_9, 
	 NULL, 	 NULL, 	 goTab_row_12, 	 NULL, 	 NULL, 
	 NULL, 	 goTab_row_16, 	 NULL, 	 NULL, 	 goTab_row_19, 
	 goTab_row_20, 	 goTab_row_21, 	 NULL, 	 NULL, 	 goTab_row_24, 
	 goTab_row_25
};
static const string actTab_row_0[] = {"5","TK_ID", "s2", "TK_LPAREN", "s1", "TK_MINUS", "s4", "TK_NUM", "s10", "TK_PRINT", "s9"};
static const string actTab_row_1[] = {"4","TK_ID", "s2", "TK_LPAREN", "s1", "TK_MINUS", "s4", "TK_NUM", "s10"};
static const string actTab_row_2[] = {"7","TK_ASSIGN", "r14", "TK_DIV", "r14", "TK_EOI", "r14", "TK_MINUS", "r14", "TK_MUL", "r14", "TK_PLUS", "r14", "TK_RPAREN", "r14"};
static const string actTab_row_3[] = {"7","TK_ASSIGN", "r11", "TK_DIV", "r11", "TK_EOI", "r11", "TK_MINUS", "r11", "TK_MUL", "r11", "TK_PLUS", "r11", "TK_RPAREN", "r11"};
static const string actTab_row_4[] = {"3","TK_ID", "s2", "TK_LPAREN", "s1", "TK_NUM", "s10"};
static const string actTab_row_5[] = {"7","TK_ASSIGN", "r9", "TK_DIV", "r9", "TK_EOI", "r9", "TK_MINUS", "r9", "TK_MUL", "r9", "TK_PLUS", "r9", "TK_RPAREN", "r9"};
static const string actTab_row_6[] = {"7","TK_ASSIGN", "r7", "TK_DIV", "s14", "TK_EOI", "r7", "TK_MINUS", "r7", "TK_MUL", "s15", "TK_PLUS", "r7", "TK_RPAREN", "r7"};
static const string actTab_row_7[] = {"4","TK_ASSIGN", "r5", "TK_EOI", "r5", "TK_MINUS", "s17", "TK_PLUS", "s18"};
static const string actTab_row_8[] = {"2","TK_ASSIGN", "s20", "TK_EOI", "r3"};
static const string actTab_row_9[] = {"4","TK_ID", "s2", "TK_LPAREN", "s1", "TK_MINUS", "s4", "TK_NUM", "s10"};
static const string actTab_row_10[] = {"7","TK_ASSIGN", "r13", "TK_DIV", "r13", "TK_EOI", "r13", "TK_MINUS", "r13", "TK_MUL", "r13", "TK_PLUS", "r13", "TK_RPAREN", "r13"};
static const string actTab_row_11[] = {"1","$", "accept"};
static const string actTab_row_12[] = {"3","TK_MINUS", "s17", "TK_PLUS", "s18", "TK_RPAREN", "s22"};
static const string actTab_row_13[] = {"7","TK_ASSIGN", "r10", "TK_DIV", "r10", "TK_EOI", "r10", "TK_MINUS", "r10", "TK_MUL", "r10", "TK_PLUS", "r10", "TK_RPAREN", "r10"};
static const string actTab_row_14[] = {"4","TK_ID", "r18", "TK_LPAREN", "r18", "TK_MINUS", "r18", "TK_NUM", "r18"};
static const string actTab_row_15[] = {"4","TK_ID", "r17", "TK_LPAREN", "r17", "TK_MINUS", "r17", "TK_NUM", "r17"};
static const string actTab_row_16[] = {"4","TK_ID", "s2", "TK_LPAREN", "s1", "TK_MINUS", "s4", "TK_NUM", "s10"};
static const string actTab_row_17[] = {"4","TK_ID", "r16", "TK_LPAREN", "r16", "TK_MINUS", "r16", "TK_NUM", "r16"};
static const string actTab_row_18[] = {"4","TK_ID", "r15", "TK_LPAREN", "r15", "TK_MINUS", "r15", "TK_NUM", "r15"};
static const string actTab_row_19[] = {"4","TK_ID", "s2", "TK_LPAREN", "s1", "TK_MINUS", "s4", "TK_NUM", "s10"};
static const string actTab_row_20[] = {"4","TK_ID", "s2", "TK_LPAREN", "s1", "TK_MINUS", "s4", "TK_NUM", "s10"};
static const string actTab_row_21[] = {"3","TK_EOI", "r2", "TK_MINUS", "s17", "TK_PLUS", "s18"};
static const string actTab_row_22[] = {"7","TK_ASSIGN", "r12", "TK_DIV", "r12", "TK_EOI", "r12", "TK_MINUS", "r12", "TK_MUL", "r12", "TK_PLUS", "r12", "TK_RPAREN", "r12"};
static const string actTab_row_23[] = {"7","TK_ASSIGN", "r8", "TK_DIV", "r8", "TK_EOI", "r8", "TK_MINUS", "r8", "TK_MUL", "r8", "TK_PLUS", "r8", "TK_RPAREN", "r8"};
static const string actTab_row_24[] = {"7","TK_ASSIGN", "r6", "TK_DIV", "s14", "TK_EOI", "r6", "TK_MINUS", "r6", "TK_MUL", "s15", "TK_PLUS", "r6", "TK_RPAREN", "r6"};
static const string actTab_row_25[] = {"4","TK_ASSIGN", "r4", "TK_EOI", "r4", "TK_MINUS", "s17", "TK_PLUS", "s18"};
static const string *actTab[] = {
	 actTab_row_0, 	 actTab_row_1, 	 actTab_row_2, 	 actTab_row_3, 	 actTab_row_4, 
	 actTab_row_5, 	 actTab_row_6, 	 actTab_row_7, 	 actTab_row_8, 	 actTab_row_9, 
	 actTab_row_10, 	 actTab_row_11, 	 actTab_row_12, 	 actTab_row_13, 	 actTab_row_14, 
	 actTab_row_15, 	 actTab_row_16, 	 actTab_row_17, 	 actTab_row_18, 	 actTab_row_19, 
	 actTab_row_20, 	 actTab_row_21, 	 actTab_row_22, 	 actTab_row_23, 	 actTab_row_24, 
	 actTab_row_25
};
static const map<string, function<AST*(vector<AST*>&)>> actions = {
	 {"binop",binop}, 
	 {"mkId",mkId}, 
	 {"mkNum",mkNum}, 
	 {"mkPrint",mkPrint}, 
	 {"pass",pass}, 
	 {"unary",unary}};
