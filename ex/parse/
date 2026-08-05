#include <vector>
#include <map>
#include <set>
#include <functional>
#include "production.hpp"
using namespace std; 
enum NTSYMBOL {
addop, args, blockexp, dec, decs, 
exp, fact, fundec, mulop, post, 
pre, pri, program, relop, rexpr, 
sp, term, ty, tydec, tyfields, 
vardec
};
map<int, Production> prod;
void initprod() {
	 prod[1]  = Production(1,"sp", SymbolString({"program"}),"");
	 prod[2]  = Production(2,"program", SymbolString({"decs","exp"}),"");
	 prod[3]  = Production(3,"decs", SymbolString({"decs","dec"}),"");
	 prod[4]  = Production(4,"dec", SymbolString({"tydec"}),"");
	 prod[5]  = Production(5,"dec", SymbolString({"fundec"}),"");
	 prod[6]  = Production(6,"dec", SymbolString({"vardec"}),"");
	 prod[7]  = Production(7,"tydec", SymbolString({"TK_TYPE","TK_ID","TK_ASSIGN","ty"}),"");
	 prod[8]  = Production(8,"ty", SymbolString({"TK_ID"}),"");
	 prod[9]  = Production(9,"ty", SymbolString({"TK_LCURLY","tyfields","TK_RCURLY"}),"");
	 prod[10]  = Production(10,"ty", SymbolString({"TK_ARRAY","TK_OF","TK_ID"}),"");
	 prod[11]  = Production(11,"tyfields", SymbolString({"TK_ID","TK_COLON","TK_ID","TK_COMMA","tyfields"}),"");
	 prod[12]  = Production(12,"tyfields", SymbolString(),"");
	 prod[13]  = Production(13,"vardec", SymbolString({"TK_VAR","TK_ID","TK_ASSIGN","exp"}),"");
	 prod[14]  = Production(14,"vardec", SymbolString({"TK_VAR","TK_ID","TK_COLON","TK_ID","TK_ASSIGN","exp"}),"");
	 prod[15]  = Production(15,"fundec", SymbolString({"TK_DEF","TK_ID","TK_LPAREN","tyfields","TK_RPAREN","TK_ASSIGN","exp"}),"");
	 prod[16]  = Production(16,"fundec", SymbolString({"TK_DEF","TK_ID","TK_LPAREN","tyfields","TK_RPAREN","TK_COLON","TK_ID","TK_ASSIGN","exp"}),"");
	 prod[17]  = Production(17,"exp", SymbolString({"exp","TK_ASSIGN","rexpr"}),"");
	 prod[18]  = Production(18,"exp", SymbolString({"rexpr"}),"");
	 prod[19]  = Production(19,"rexpr", SymbolString({"rexpr","relop","term"}),"");
	 prod[20]  = Production(20,"rexpr", SymbolString({"term"}),"");
	 prod[21]  = Production(21,"term", SymbolString({"term","addop","fact"}),"");
	 prod[22]  = Production(22,"term", SymbolString({"fact"}),"");
	 prod[23]  = Production(23,"fact", SymbolString({"fact","mulop","pre"}),"");
	 prod[24]  = Production(24,"pre", SymbolString({"TK_MINUS","post"}),"");
	 prod[25]  = Production(25,"pre", SymbolString({"TK_NOT","post"}),"");
	 prod[26]  = Production(26,"pre", SymbolString({"post"}),"");
	 prod[27]  = Production(27,"post", SymbolString({"pri","TK_LPAREN","args","TK_RPAREN"}),"");
	 prod[28]  = Production(28,"post", SymbolString({"pri","TK_LSQB","exp","TK_RPAREN"}),"");
	 prod[29]  = Production(29,"post", SymbolString({"pri"}),"");
	 prod[30]  = Production(30,"pri", SymbolString({"TK_ID"}),"@mkId");
	 prod[31]  = Production(31,"pri", SymbolString({"TK_NUM"}),"@mkNum");
	 prod[32]  = Production(32,"pri", SymbolString({"TK_LPAREN","exp","TK_RPAREN"}),"@pass");
	 prod[33]  = Production(33,"pri", SymbolString({"blockexp"}),"");
	 prod[34]  = Production(34,"blockexp", SymbolString({"TK_LET","decs","TK_IN","exp","TK_END"}),"");
	 prod[35]  = Production(35,"addop", SymbolString({"TK_MINUS"}),"");
	 prod[36]  = Production(36,"addop", SymbolString({"TK_PLUS"}),"");
	 prod[37]  = Production(37,"mulop", SymbolString({"TK_MUL"}),"");
	 prod[38]  = Production(38,"mulop", SymbolString({"TK_DIV"}),"");
	 prod[39]  = Production(39,"mulop", SymbolString({"TK_MOD"}),"");
	 prod[40]  = Production(40,"relop", SymbolString({"TK_EQU"}),"");
	 prod[41]  = Production(41,"relop", SymbolString({"TK_NEQ"}),"");
	 prod[42]  = Production(42,"relop", SymbolString({"TK_LT"}),"");
	 prod[43]  = Production(43,"relop", SymbolString({"TK_GT"}),"");
	 prod[44]  = Production(44,"relop", SymbolString({"TK_LTE"}),"");
	 prod[45]  = Production(45,"relop", SymbolString({"TK_GTE"}),"");
}
map<int,map<string,string>> goTab;
void initgoTab() {
	 goTab[0] = {{"decs", "1"}, {"program", "2"}};
	 goTab[1] = {{"dec", "11"}, {"exp", "13"}, {"fact", "12"}, {"fundec", "6"}, {"rexpr", "8"}, {"term", "10"}, {"tydec", "7"}, {"vardec", "5"}};
	 goTab[8] = {{"relop", "22"}};
	 goTab[10] = {{"addop", "26"}};
	 goTab[12] = {{"mulop", "30"}};
	 goTab[22] = {{"fact", "12"}, {"term", "34"}};
	 goTab[26] = {{"fact", "37"}};
	 goTab[30] = {{"blockexp", "39"}, {"post", "45"}, {"pre", "47"}, {"pri", "43"}};
	 goTab[31] = {{"fact", "12"}, {"rexpr", "48"}, {"term", "10"}};
	 goTab[32] = {{"tyfields", "50"}};
	 goTab[33] = {{"ty", "54"}};
	 goTab[34] = {{"addop", "26"}};
	 goTab[36] = {{"exp", "56"}, {"fact", "12"}, {"rexpr", "8"}, {"term", "10"}};
	 goTab[37] = {{"mulop", "30"}};
	 goTab[38] = {{"decs", "57"}};
	 goTab[40] = {{"exp", "58"}, {"fact", "12"}, {"rexpr", "8"}, {"term", "10"}};
	 goTab[44] = {{"blockexp", "39"}, {"post", "61"}, {"pri", "43"}};
	 goTab[46] = {{"blockexp", "39"}, {"post", "62"}, {"pri", "43"}};
	 goTab[48] = {{"relop", "22"}};
	 goTab[52] = {{"tyfields", "66"}};
	 goTab[57] = {{"dec", "11"}, {"fundec", "6"}, {"tydec", "7"}, {"vardec", "5"}};
	 goTab[59] = {{"exp", "70"}, {"fact", "12"}, {"rexpr", "8"}, {"term", "10"}};
	 goTab[60] = {{"args", "71"}};
	 goTab[67] = {{"exp", "77"}, {"fact", "12"}, {"rexpr", "8"}, {"term", "10"}};
	 goTab[68] = {{"exp", "78"}, {"fact", "12"}, {"rexpr", "8"}, {"term", "10"}};
	 goTab[74] = {{"exp", "83"}, {"fact", "12"}, {"rexpr", "8"}, {"term", "10"}};
	 goTab[81] = {{"tyfields", "85"}};
	 goTab[86] = {{"exp", "87"}, {"fact", "12"}, {"rexpr", "8"}, {"term", "10"}};
}
map<int,map<string,string>> actTab;
void initactTab() {
	 actTab[1] = {{"TK_DEF", "s3"}, {"TK_TYPE", "s4"}, {"TK_VAR", "s9"}};
	 actTab[2] = {{"$", "accept"}};
	 actTab[3] = {{"TK_ID", "s14"}};
	 actTab[4] = {{"TK_ID", "s15"}};
	 actTab[5] = {{"TK_DEF", "r6"}, {"TK_IN", "r6"}, {"TK_TYPE", "r6"}, {"TK_VAR", "r6"}};
	 actTab[6] = {{"TK_DEF", "r5"}, {"TK_IN", "r5"}, {"TK_TYPE", "r5"}, {"TK_VAR", "r5"}};
	 actTab[7] = {{"TK_DEF", "r4"}, {"TK_IN", "r4"}, {"TK_TYPE", "r4"}, {"TK_VAR", "r4"}};
	 actTab[8] = {{"TK_ASSIGN", "r18"}, {"TK_DEF", "r18"}, {"TK_END", "r18"}, {"TK_EOI", "r18"}, {"TK_EQU", "s20"}, {"TK_GT", "s18"}, {"TK_GTE", "s16"}, {"TK_IN", "r18"}, {"TK_LT", "s21"}, {"TK_LTE", "s17"}, {"TK_NEQ", "s19"}, {"TK_RPAREN", "r18"}, {"TK_TYPE", "r18"}, {"TK_VAR", "r18"}};
	 actTab[9] = {{"TK_ID", "s23"}};
	 actTab[10] = {{"TK_ASSIGN", "r20"}, {"TK_DEF", "r20"}, {"TK_END", "r20"}, {"TK_EOI", "r20"}, {"TK_EQU", "r20"}, {"TK_GT", "r20"}, {"TK_GTE", "r20"}, {"TK_IN", "r20"}, {"TK_LT", "r20"}, {"TK_LTE", "r20"}, {"TK_MINUS", "s25"}, {"TK_NEQ", "r20"}, {"TK_PLUS", "s24"}, {"TK_RPAREN", "r20"}, {"TK_TYPE", "r20"}, {"TK_VAR", "r20"}};
	 actTab[11] = {{"TK_DEF", "r3"}, {"TK_IN", "r3"}, {"TK_TYPE", "r3"}, {"TK_VAR", "r3"}};
	 actTab[12] = {{"TK_ASSIGN", "r22"}, {"TK_DEF", "r22"}, {"TK_DIV", "s28"}, {"TK_END", "r22"}, {"TK_EOI", "r22"}, {"TK_EQU", "r22"}, {"TK_GT", "r22"}, {"TK_GTE", "r22"}, {"TK_IN", "r22"}, {"TK_LT", "r22"}, {"TK_LTE", "r22"}, {"TK_MINUS", "r22"}, {"TK_MOD", "s27"}, {"TK_MUL", "s29"}, {"TK_NEQ", "r22"}, {"TK_PLUS", "r22"}, {"TK_RPAREN", "r22"}, {"TK_TYPE", "r22"}, {"TK_VAR", "r22"}};
	 actTab[13] = {{"TK_ASSIGN", "s31"}, {"TK_EOI", "r2"}};
	 actTab[14] = {{"TK_LPAREN", "s32"}};
	 actTab[15] = {{"TK_ASSIGN", "s33"}};
	 actTab[23] = {{"TK_ASSIGN", "s36"}, {"TK_COLON", "s35"}};
	 actTab[27] = {{"TK_ID", "r39"}, {"TK_LET", "r39"}, {"TK_LPAREN", "r39"}, {"TK_MINUS", "r39"}, {"TK_NOT", "r39"}, {"TK_NUM", "r39"}};
	 actTab[28] = {{"TK_ID", "r38"}, {"TK_LET", "r38"}, {"TK_LPAREN", "r38"}, {"TK_MINUS", "r38"}, {"TK_NOT", "r38"}, {"TK_NUM", "r38"}};
	 actTab[29] = {{"TK_ID", "r37"}, {"TK_LET", "r37"}, {"TK_LPAREN", "r37"}, {"TK_MINUS", "r37"}, {"TK_NOT", "r37"}, {"TK_NUM", "r37"}};
	 actTab[30] = {{"TK_ID", "s41"}, {"TK_LET", "s38"}, {"TK_LPAREN", "s40"}, {"TK_MINUS", "s46"}, {"TK_NOT", "s44"}, {"TK_NUM", "s42"}};
	 actTab[32] = {{"TK_ID", "s49"}, {"TK_RPAREN", "r12"}};
	 actTab[33] = {{"TK_ARRAY", "s51"}, {"TK_ID", "s53"}, {"TK_LCURLY", "s52"}};
	 actTab[34] = {{"TK_ASSIGN", "r19"}, {"TK_DEF", "r19"}, {"TK_END", "r19"}, {"TK_EOI", "r19"}, {"TK_EQU", "r19"}, {"TK_GT", "r19"}, {"TK_GTE", "r19"}, {"TK_IN", "r19"}, {"TK_LT", "r19"}, {"TK_LTE", "r19"}, {"TK_MINUS", "s25"}, {"TK_NEQ", "r19"}, {"TK_PLUS", "s24"}, {"TK_RPAREN", "r19"}, {"TK_TYPE", "r19"}, {"TK_VAR", "r19"}};
	 actTab[35] = {{"TK_ID", "s55"}};
	 actTab[37] = {{"TK_ASSIGN", "r21"}, {"TK_DEF", "r21"}, {"TK_DIV", "s28"}, {"TK_END", "r21"}, {"TK_EOI", "r21"}, {"TK_EQU", "r21"}, {"TK_GT", "r21"}, {"TK_GTE", "r21"}, {"TK_IN", "r21"}, {"TK_LT", "r21"}, {"TK_LTE", "r21"}, {"TK_MINUS", "r21"}, {"TK_MOD", "s27"}, {"TK_MUL", "s29"}, {"TK_NEQ", "r21"}, {"TK_PLUS", "r21"}, {"TK_RPAREN", "r21"}, {"TK_TYPE", "r21"}, {"TK_VAR", "r21"}};
	 actTab[39] = {{"TK_ASSIGN", "r33"}, {"TK_DEF", "r33"}, {"TK_DIV", "r33"}, {"TK_END", "r33"}, {"TK_EOI", "r33"}, {"TK_EQU", "r33"}, {"TK_GT", "r33"}, {"TK_GTE", "r33"}, {"TK_IN", "r33"}, {"TK_LPAREN", "r33"}, {"TK_LSQB", "r33"}, {"TK_LT", "r33"}, {"TK_LTE", "r33"}, {"TK_MINUS", "r33"}, {"TK_MOD", "r33"}, {"TK_MUL", "r33"}, {"TK_NEQ", "r33"}, {"TK_PLUS", "r33"}, {"TK_RPAREN", "r33"}, {"TK_TYPE", "r33"}, {"TK_VAR", "r33"}};
	 actTab[41] = {{"TK_ASSIGN", "r30"}, {"TK_DEF", "r30"}, {"TK_DIV", "r30"}, {"TK_END", "r30"}, {"TK_EOI", "r30"}, {"TK_EQU", "r30"}, {"TK_GT", "r30"}, {"TK_GTE", "r30"}, {"TK_IN", "r30"}, {"TK_LPAREN", "r30"}, {"TK_LSQB", "r30"}, {"TK_LT", "r30"}, {"TK_LTE", "r30"}, {"TK_MINUS", "r30"}, {"TK_MOD", "r30"}, {"TK_MUL", "r30"}, {"TK_NEQ", "r30"}, {"TK_PLUS", "r30"}, {"TK_RPAREN", "r30"}, {"TK_TYPE", "r30"}, {"TK_VAR", "r30"}};
	 actTab[42] = {{"TK_ASSIGN", "r31"}, {"TK_DEF", "r31"}, {"TK_DIV", "r31"}, {"TK_END", "r31"}, {"TK_EOI", "r31"}, {"TK_EQU", "r31"}, {"TK_GT", "r31"}, {"TK_GTE", "r31"}, {"TK_IN", "r31"}, {"TK_LPAREN", "r31"}, {"TK_LSQB", "r31"}, {"TK_LT", "r31"}, {"TK_LTE", "r31"}, {"TK_MINUS", "r31"}, {"TK_MOD", "r31"}, {"TK_MUL", "r31"}, {"TK_NEQ", "r31"}, {"TK_PLUS", "r31"}, {"TK_RPAREN", "r31"}, {"TK_TYPE", "r31"}, {"TK_VAR", "r31"}};
	 actTab[43] = {{"TK_ASSIGN", "r29"}, {"TK_DEF", "r29"}, {"TK_DIV", "r29"}, {"TK_END", "r29"}, {"TK_EOI", "r29"}, {"TK_EQU", "r29"}, {"TK_GT", "r29"}, {"TK_GTE", "r29"}, {"TK_IN", "r29"}, {"TK_LPAREN", "s60"}, {"TK_LSQB", "s59"}, {"TK_LT", "r29"}, {"TK_LTE", "r29"}, {"TK_MINUS", "r29"}, {"TK_MOD", "r29"}, {"TK_MUL", "r29"}, {"TK_NEQ", "r29"}, {"TK_PLUS", "r29"}, {"TK_RPAREN", "r29"}, {"TK_TYPE", "r29"}, {"TK_VAR", "r29"}};
	 actTab[44] = {{"TK_ID", "s41"}, {"TK_LET", "s38"}, {"TK_LPAREN", "s40"}, {"TK_NUM", "s42"}};
	 actTab[45] = {{"TK_ASSIGN", "r26"}, {"TK_DEF", "r26"}, {"TK_DIV", "r26"}, {"TK_END", "r26"}, {"TK_EOI", "r26"}, {"TK_EQU", "r26"}, {"TK_GT", "r26"}, {"TK_GTE", "r26"}, {"TK_IN", "r26"}, {"TK_LT", "r26"}, {"TK_LTE", "r26"}, {"TK_MINUS", "r26"}, {"TK_MOD", "r26"}, {"TK_MUL", "r26"}, {"TK_NEQ", "r26"}, {"TK_PLUS", "r26"}, {"TK_RPAREN", "r26"}, {"TK_TYPE", "r26"}, {"TK_VAR", "r26"}};
	 actTab[46] = {{"TK_ID", "s41"}, {"TK_LET", "s38"}, {"TK_LPAREN", "s40"}, {"TK_NUM", "s42"}};
	 actTab[47] = {{"TK_ASSIGN", "r23"}, {"TK_DEF", "r23"}, {"TK_DIV", "r23"}, {"TK_END", "r23"}, {"TK_EOI", "r23"}, {"TK_EQU", "r23"}, {"TK_GT", "r23"}, {"TK_GTE", "r23"}, {"TK_IN", "r23"}, {"TK_LT", "r23"}, {"TK_LTE", "r23"}, {"TK_MINUS", "r23"}, {"TK_MOD", "r23"}, {"TK_MUL", "r23"}, {"TK_NEQ", "r23"}, {"TK_PLUS", "r23"}, {"TK_RPAREN", "r23"}, {"TK_TYPE", "r23"}, {"TK_VAR", "r23"}};
	 actTab[48] = {{"TK_ASSIGN", "r17"}, {"TK_DEF", "r17"}, {"TK_END", "r17"}, {"TK_EOI", "r17"}, {"TK_EQU", "s20"}, {"TK_GT", "s18"}, {"TK_GTE", "s16"}, {"TK_IN", "r17"}, {"TK_LT", "s21"}, {"TK_LTE", "s17"}, {"TK_NEQ", "s19"}, {"TK_RPAREN", "r17"}, {"TK_TYPE", "r17"}, {"TK_VAR", "r17"}};
	 actTab[49] = {{"TK_COLON", "s63"}};
	 actTab[50] = {{"TK_RPAREN", "s64"}};
	 actTab[51] = {{"TK_OF", "s65"}};
	 actTab[52] = {{"TK_ID", "s49"}, {"TK_RCURLY", "r12"}};
	 actTab[53] = {{"TK_DEF", "r8"}, {"TK_IN", "r8"}, {"TK_TYPE", "r8"}, {"TK_VAR", "r8"}};
	 actTab[54] = {{"TK_DEF", "r7"}, {"TK_IN", "r7"}, {"TK_TYPE", "r7"}, {"TK_VAR", "r7"}};
	 actTab[55] = {{"TK_ASSIGN", "s67"}};
	 actTab[56] = {{"TK_ASSIGN", "s31"}, {"TK_DEF", "r13"}, {"TK_IN", "r13"}, {"TK_TYPE", "r13"}, {"TK_VAR", "r13"}};
	 actTab[57] = {{"TK_DEF", "s3"}, {"TK_IN", "s68"}, {"TK_TYPE", "s4"}, {"TK_VAR", "s9"}};
	 actTab[58] = {{"TK_ASSIGN", "s31"}, {"TK_RPAREN", "s69"}};
	 actTab[61] = {{"TK_ASSIGN", "r25"}, {"TK_DEF", "r25"}, {"TK_DIV", "r25"}, {"TK_END", "r25"}, {"TK_EOI", "r25"}, {"TK_EQU", "r25"}, {"TK_GT", "r25"}, {"TK_GTE", "r25"}, {"TK_IN", "r25"}, {"TK_LT", "r25"}, {"TK_LTE", "r25"}, {"TK_MINUS", "r25"}, {"TK_MOD", "r25"}, {"TK_MUL", "r25"}, {"TK_NEQ", "r25"}, {"TK_PLUS", "r25"}, {"TK_RPAREN", "r25"}, {"TK_TYPE", "r25"}, {"TK_VAR", "r25"}};
	 actTab[62] = {{"TK_ASSIGN", "r24"}, {"TK_DEF", "r24"}, {"TK_DIV", "r24"}, {"TK_END", "r24"}, {"TK_EOI", "r24"}, {"TK_EQU", "r24"}, {"TK_GT", "r24"}, {"TK_GTE", "r24"}, {"TK_IN", "r24"}, {"TK_LT", "r24"}, {"TK_LTE", "r24"}, {"TK_MINUS", "r24"}, {"TK_MOD", "r24"}, {"TK_MUL", "r24"}, {"TK_NEQ", "r24"}, {"TK_PLUS", "r24"}, {"TK_RPAREN", "r24"}, {"TK_TYPE", "r24"}, {"TK_VAR", "r24"}};
	 actTab[63] = {{"TK_ID", "s72"}};
	 actTab[64] = {{"TK_ASSIGN", "s74"}, {"TK_COLON", "s73"}};
	 actTab[65] = {{"TK_ID", "s75"}};
	 actTab[66] = {{"TK_RCURLY", "s76"}};
	 actTab[69] = {{"TK_ASSIGN", "r32"}, {"TK_DEF", "r32"}, {"TK_DIV", "r32"}, {"TK_END", "r32"}, {"TK_EOI", "r32"}, {"TK_EQU", "r32"}, {"TK_GT", "r32"}, {"TK_GTE", "r32"}, {"TK_IN", "r32"}, {"TK_LPAREN", "r32"}, {"TK_LSQB", "r32"}, {"TK_LT", "r32"}, {"TK_LTE", "r32"}, {"TK_MINUS", "r32"}, {"TK_MOD", "r32"}, {"TK_MUL", "r32"}, {"TK_NEQ", "r32"}, {"TK_PLUS", "r32"}, {"TK_RPAREN", "r32"}, {"TK_TYPE", "r32"}, {"TK_VAR", "r32"}};
	 actTab[70] = {{"TK_ASSIGN", "s31"}, {"TK_RPAREN", "s79"}};
	 actTab[71] = {{"TK_RPAREN", "s80"}};
	 actTab[72] = {{"TK_COMMA", "s81"}};
	 actTab[73] = {{"TK_ID", "s82"}};
	 actTab[75] = {{"TK_DEF", "r10"}, {"TK_IN", "r10"}, {"TK_TYPE", "r10"}, {"TK_VAR", "r10"}};
	 actTab[76] = {{"TK_DEF", "r9"}, {"TK_IN", "r9"}, {"TK_TYPE", "r9"}, {"TK_VAR", "r9"}};
	 actTab[77] = {{"TK_ASSIGN", "s31"}, {"TK_DEF", "r14"}, {"TK_IN", "r14"}, {"TK_TYPE", "r14"}, {"TK_VAR", "r14"}};
	 actTab[78] = {{"TK_ASSIGN", "s31"}, {"TK_END", "s84"}};
	 actTab[79] = {{"TK_ASSIGN", "r28"}, {"TK_DEF", "r28"}, {"TK_DIV", "r28"}, {"TK_END", "r28"}, {"TK_EOI", "r28"}, {"TK_EQU", "r28"}, {"TK_GT", "r28"}, {"TK_GTE", "r28"}, {"TK_IN", "r28"}, {"TK_LT", "r28"}, {"TK_LTE", "r28"}, {"TK_MINUS", "r28"}, {"TK_MOD", "r28"}, {"TK_MUL", "r28"}, {"TK_NEQ", "r28"}, {"TK_PLUS", "r28"}, {"TK_RPAREN", "r28"}, {"TK_TYPE", "r28"}, {"TK_VAR", "r28"}};
	 actTab[80] = {{"TK_ASSIGN", "r27"}, {"TK_DEF", "r27"}, {"TK_DIV", "r27"}, {"TK_END", "r27"}, {"TK_EOI", "r27"}, {"TK_EQU", "r27"}, {"TK_GT", "r27"}, {"TK_GTE", "r27"}, {"TK_IN", "r27"}, {"TK_LT", "r27"}, {"TK_LTE", "r27"}, {"TK_MINUS", "r27"}, {"TK_MOD", "r27"}, {"TK_MUL", "r27"}, {"TK_NEQ", "r27"}, {"TK_PLUS", "r27"}, {"TK_RPAREN", "r27"}, {"TK_TYPE", "r27"}, {"TK_VAR", "r27"}};
	 actTab[81] = {{"TK_ID", "s49"}, {"TK_RCURLY", "r12"}, {"TK_RPAREN", "r12"}};
	 actTab[82] = {{"TK_ASSIGN", "s86"}};
	 actTab[83] = {{"TK_ASSIGN", "s31"}, {"TK_DEF", "r15"}, {"TK_IN", "r15"}, {"TK_TYPE", "r15"}, {"TK_VAR", "r15"}};
	 actTab[84] = {{"TK_ASSIGN", "r34"}, {"TK_DEF", "r34"}, {"TK_DIV", "r34"}, {"TK_END", "r34"}, {"TK_EOI", "r34"}, {"TK_EQU", "r34"}, {"TK_GT", "r34"}, {"TK_GTE", "r34"}, {"TK_IN", "r34"}, {"TK_LPAREN", "r34"}, {"TK_LSQB", "r34"}, {"TK_LT", "r34"}, {"TK_LTE", "r34"}, {"TK_MINUS", "r34"}, {"TK_MOD", "r34"}, {"TK_MUL", "r34"}, {"TK_NEQ", "r34"}, {"TK_PLUS", "r34"}, {"TK_RPAREN", "r34"}, {"TK_TYPE", "r34"}, {"TK_VAR", "r34"}};
	 actTab[85] = {{"TK_RCURLY", "r11"}, {"TK_RPAREN", "r11"}};
	 actTab[87] = {{"TK_ASSIGN", "s31"}, {"TK_DEF", "r16"}, {"TK_IN", "r16"}, {"TK_TYPE", "r16"}, {"TK_VAR", "r16"}};
}
map<string, function<AST*(vector<AST*>&)>> actions;
void initActions() {
	 actions.insert({"mkId",mkId});
	 actions.insert({"mkNum",mkNum});
	 actions.insert({"pass",pass});
}
