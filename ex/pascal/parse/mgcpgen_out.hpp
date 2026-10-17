#include <vector>
#include <map>
#include <set>
#include <functional>
using namespace std; 
enum NTSYMBOL {
	 DOLLARACCEPT = 52,
TK_IF, TK_DO, TK_OF, TK_OR, TK_AND, TK_END, 
TK_MOD, TK_VAR, TK_ELSE, TK_REAL, TK_THEN, 
TK_TYPE, TK_ARRAY, TK_BEGIN, TK_WHILE, TK_RECORD, 
TK_RETURN, TK_INTEGER, TK_PROGRAM, TK_PRINT, TK_DEF, 
TK_LPAREN, TK_RPAREN, TK_LSQB, TK_RSQB, TK_PLUS, 
TK_MINUS, TK_MUL, TK_DIV, TK_LT, TK_GT, 
TK_SEMI, TK_COLON, TK_ASSIGN, TK_EQU, TK_NEQ, 
TK_COMMA, TK_RANGE, TK_PERIOD, TK_NUM, TK_ID, 
TK_STRING, sp, PROG, PROGHEADER, 
DECLLIST, SUBDECLLIST, PROGT, IDENTLIST, COMPOUNDSTMT, 
PRI, DECLSTMT, TYPE, FIELDLIST, STANDTYPE, 
EXPR, RECFIELD, SUBDECL, SUBHEAD, ARGLIST, 
PARAMLIST, OPTSTMT, STMTLIST, STMT, EXPRSTMT, 
IFSTMT, WHILESTMT, PRINTSTMT, RETSTMT, AEXPR, 
EXPRLIST, SIMPEXPR, RELOP, ADDOP, TERM, 
MULOP, FACT, POST
};
struct Production {
	 int id;
	 int lhs;
	 vector<int> rhs;
	 string actsym;
 }; 

static const Production prod[] = {
 	 {0, 0, {}, ""},

	 {1,sp, {PROG},""}, 
	 {2,PROG, {PROGHEADER, DECLLIST, SUBDECLLIST, PROGT},"9"}, 
	 {3,PROGHEADER, {TK_PROGRAM, TK_PROGRAM, TK_ID, TK_ID, TK_LPAREN, TK_LPAREN, IDENTLIST, TK_RPAREN, TK_RPAREN, TK_SEMI, TK_SEMI},";"}, 
	 {4,PROGT, {COMPOUNDSTMT, TK_PERIOD, TK_PERIOD},"="}, 
	 {5,IDENTLIST, {IDENTLIST, TK_COMMA, TK_COMMA, PRI},"?"}, 
	 {6,IDENTLIST, {PRI},""}, 
	 {7,DECLLIST, {DECLSTMT, DECLLIST},"?"}, 
	 {8,DECLLIST, {#, #},""}, 
	 {9,DECLSTMT, {TK_VAR, TK_VAR, IDENTLIST, TK_COLON, TK_COLON, TYPE, TK_SEMI, TK_SEMI},"B"}, 
	 {10,DECLSTMT, {TK_TYPE, TK_TYPE, TK_ID, TK_ID, TK_ASSIGN, TK_ASSIGN, TK_RECORD, TK_RECORD, FIELDLIST, TK_END, TK_END, TK_SEMI, TK_SEMI},"D"}, 
	 {11,TYPE, {STANDTYPE},""}, 
	 {12,TYPE, {TK_ARRAY, TK_ARRAY, TK_LSQB, TK_LSQB, EXPR, TK_RANGE, TK_RANGE, EXPR, TK_RSQB, TK_RSQB, TK_OF, TK_OF, STANDTYPE},"G"}, 
	 {13,FIELDLIST, {RECFIELD, FIELDLIST},"?"}, 
	 {14,FIELDLIST, {RECFIELD},""}, 
	 {15,RECFIELD, {TK_ID, TK_ID, TK_COLON, TK_COLON, TYPE, TK_SEMI, TK_SEMI},"I"}, 
	 {16,STANDTYPE, {TK_INTEGER, TK_INTEGER},"J"}, 
	 {17,STANDTYPE, {TK_REAL, TK_REAL},"J"}, 
	 {18,STANDTYPE, {TK_STRING, TK_STRING},"J"}, 
	 {19,STANDTYPE, {TK_ID, TK_ID},"J"}, 
	 {20,SUBDECLLIST, {SUBDECL, TK_SEMI, TK_SEMI, SUBDECLLIST},"?"}, 
	 {21,SUBDECLLIST, {#, #},""}, 
	 {22,SUBDECL, {SUBHEAD, DECLLIST, COMPOUNDSTMT},"M"}, 
	 {23,SUBHEAD, {TK_DEF, TK_DEF, TK_ID, TK_ID, ARGLIST, TK_COLON, TK_COLON, TYPE, TK_SEMI, TK_SEMI},"O"}, 
	 {24,ARGLIST, {TK_LPAREN, TK_LPAREN, PARAMLIST, TK_RPAREN, TK_RPAREN},"Q"}, 
	 {25,PARAMLIST, {PARAMLIST, TK_SEMI, TK_SEMI, IDENTLIST, TK_COLON, TK_COLON, TYPE},"?"}, 
	 {26,PARAMLIST, {IDENTLIST, TK_COLON, TK_COLON, TYPE},""}, 
	 {27,COMPOUNDSTMT, {TK_BEGIN, TK_BEGIN, OPTSTMT, TK_END, TK_END},"S"}, 
	 {28,OPTSTMT, {STMTLIST},""}, 
	 {29,OPTSTMT, {#, #},""}, 
	 {30,STMTLIST, {STMTLIST, TK_SEMI, TK_SEMI, STMT},"?"}, 
	 {31,STMTLIST, {STMT},""}, 
	 {32,STMT, {EXPRSTMT},""}, 
	 {33,STMT, {COMPOUNDSTMT},""}, 
	 {34,STMT, {IFSTMT},""}, 
	 {35,STMT, {WHILESTMT},""}, 
	 {36,STMT, {PRINTSTMT},""}, 
	 {37,STMT, {RETSTMT},""}, 
	 {38,EXPRSTMT, {AEXPR},""}, 
	 {39,IFSTMT, {TK_IF, TK_IF, EXPR, TK_THEN, TK_THEN, STMT, TK_ELSE, TK_ELSE, STMT},""}, 
	 {40,WHILESTMT, {TK_WHILE, TK_WHILE, EXPR, TK_DO, TK_DO, STMT},"]"}, 
	 {41,PRINTSTMT, {TK_PRINT, TK_PRINT, EXPR},"^"}, 
	 {42,RETSTMT, {TK_RETURN, TK_RETURN, EXPR},"_"}, 
	 {43,EXPRLIST, {EXPRLIST, TK_COMMA, TK_COMMA, EXPR},"?"}, 
	 {44,EXPRLIST, {EXPR},""}, 
	 {45,AEXPR, {EXPR, TK_ASSIGN, TK_ASSIGN, EXPR},"a"}, 
	 {46,AEXPR, {EXPR},""}, 
	 {47,EXPR, {SIMPEXPR, RELOP, SIMPEXPR},"a"}, 
	 {48,EXPR, {SIMPEXPR},""}, 
	 {49,SIMPEXPR, {SIMPEXPR, ADDOP, TERM},"a"}, 
	 {50,SIMPEXPR, {TERM},""}, 
	 {51,TERM, {TERM, MULOP, FACT},"a"}, 
	 {52,TERM, {FACT},""}, 
	 {53,FACT, {TK_MINUS, TK_MINUS, POST},"i"}, 
	 {54,FACT, {POST},""}, 
	 {55,POST, {PRI, TK_LPAREN, TK_LPAREN, EXPRLIST, TK_RPAREN, TK_RPAREN},"j"}, 
	 {56,POST, {PRI, TK_LSQB, TK_LSQB, EXPR, TK_RSQB, TK_RSQB},"k"}, 
	 {57,POST, {PRI, TK_PERIOD, TK_PERIOD, PRI},"k"}, 
	 {58,POST, {PRI},""}, 
	 {59,PRI, {TK_ID, TK_ID},"l"}, 
	 {60,PRI, {TK_NUM, TK_NUM},"m"}, 
	 {61,PRI, {TK_STRING, TK_STRING},"n"}, 
	 {62,PRI, {TK_LPAREN, TK_LPAREN, EXPR, TK_RPAREN, TK_RPAREN},"Q"}, 
	 {63,RELOP, {TK_LT, TK_LT},""}, 
	 {64,RELOP, {TK_GT, TK_GT},""}, 
	 {65,RELOP, {TK_EQU, TK_EQU},""}, 
	 {66,RELOP, {TK_NEQ, TK_NEQ},""}, 
	 {67,ADDOP, {TK_PLUS, TK_PLUS},""}, 
	 {68,ADDOP, {TK_MINUS, TK_MINUS},""}, 
	 {69,ADDOP, {TK_OR, TK_OR},""}, 
	 {70,MULOP, {TK_MUL, TK_MUL},""}, 
	 {71,MULOP, {TK_DIV, TK_DIV},""}, 
	 {72,MULOP, {TK_MOD, TK_MOD},""}, 
	 {73,MULOP, {TK_AND, TK_AND},""}
};
static const int goTab_row_0[] = {3,TK_PROGRAM, 1, PROG, 3, PROGHEADER, 2};
static const int goTab_row_1[] = {1,TK_PROGRAM, 4};
static const int goTab_row_2[] = {5,TK_VAR, 5, TK_TYPE, 7, #, 6, DECLLIST, 9, DECLSTMT, 8};
static const int goTab_row_4[] = {1,TK_ID, 10};
static const int goTab_row_5[] = {1,TK_VAR, 11};
static const int goTab_row_6[] = {1,#, 12};
static const int goTab_row_7[] = {1,TK_TYPE, 13};
static const int goTab_row_8[] = {5,TK_VAR, 5, TK_TYPE, 7, #, 6, DECLLIST, 14, DECLSTMT, 8};
static const int goTab_row_9[] = {5,TK_DEF, 15, #, 17, SUBDECLLIST, 19, SUBDECL, 18, SUBHEAD, 16};
static const int goTab_row_10[] = {1,TK_ID, 20};
static const int goTab_row_11[] = {6,TK_LPAREN, 23, TK_NUM, 25, TK_ID, 22, TK_STRING, 21, IDENTLIST, 26, PRI, 24};
static const int goTab_row_13[] = {1,TK_ID, 27};
static const int goTab_row_15[] = {1,TK_DEF, 28};
static const int goTab_row_16[] = {5,TK_VAR, 5, TK_TYPE, 7, #, 6, DECLLIST, 29, DECLSTMT, 8};
static const int goTab_row_17[] = {1,#, 30};
static const int goTab_row_18[] = {1,TK_SEMI, 31};
static const int goTab_row_19[] = {3,TK_BEGIN, 32, PROGT, 34, COMPOUNDSTMT, 33};
static const int goTab_row_20[] = {1,TK_LPAREN, 35};
static const int goTab_row_21[] = {1,TK_STRING, 36};
static const int goTab_row_22[] = {1,TK_ID, 37};
static const int goTab_row_23[] = {1,TK_LPAREN, 38};
static const int goTab_row_25[] = {1,TK_NUM, 39};
static const int goTab_row_26[] = {2,TK_COLON, 41, TK_COMMA, 40};
static const int goTab_row_27[] = {1,TK_ID, 42};
static const int goTab_row_28[] = {1,TK_ID, 43};
static const int goTab_row_29[] = {2,TK_BEGIN, 32, COMPOUNDSTMT, 44};
static const int goTab_row_31[] = {1,TK_SEMI, 45};
static const int goTab_row_32[] = {1,TK_BEGIN, 46};
static const int goTab_row_33[] = {1,TK_PERIOD, 47};
static const int goTab_row_35[] = {1,TK_LPAREN, 48};
static const int goTab_row_38[] = {11,TK_LPAREN, 23, TK_MINUS, 50, TK_NUM, 25, TK_ID, 22, TK_STRING, 21, PRI, 52, EXPR, 55, SIMPEXPR, 54, TERM, 53, FACT, 51, POST, 49};
static const int goTab_row_40[] = {1,TK_COMMA, 56};
static const int goTab_row_41[] = {1,TK_COLON, 57};
static const int goTab_row_42[] = {1,TK_ASSIGN, 58};
static const int goTab_row_43[] = {1,TK_ID, 59};
static const int goTab_row_45[] = {5,TK_DEF, 15, #, 17, SUBDECLLIST, 60, SUBDECL, 18, SUBHEAD, 16};
static const int goTab_row_46[] = {27,TK_IF, 76, TK_BEGIN, 32, TK_WHILE, 64, TK_RETURN, 62, TK_PRINT, 63, TK_LPAREN, 23, TK_MINUS, 50, TK_NUM, 25, TK_ID, 22, TK_STRING, 21, #, 67, COMPOUNDSTMT, 71, PRI, 52, EXPR, 61, OPTSTMT, 65, STMTLIST, 66, STMT, 69, EXPRSTMT, 70, IFSTMT, 75, WHILESTMT, 68, PRINTSTMT, 72, RETSTMT, 73, AEXPR, 74, SIMPEXPR, 54, TERM, 53, FACT, 51, POST, 49};
static const int goTab_row_47[] = {1,TK_PERIOD, 77};
static const int goTab_row_48[] = {6,TK_LPAREN, 23, TK_NUM, 25, TK_ID, 22, TK_STRING, 21, IDENTLIST, 78, PRI, 24};
static const int goTab_row_50[] = {1,TK_MINUS, 79};
static const int goTab_row_52[] = {3,TK_LPAREN, 82, TK_LSQB, 81, TK_PERIOD, 80};
static const int goTab_row_53[] = {5,TK_AND, 84, TK_MOD, 83, TK_MUL, 86, TK_DIV, 85, MULOP, 87};
static const int goTab_row_54[] = {9,TK_OR, 88, TK_PLUS, 90, TK_MINUS, 89, TK_LT, 94, TK_GT, 93, TK_EQU, 92, TK_NEQ, 91, RELOP, 96, ADDOP, 95};
static const int goTab_row_55[] = {1,TK_RPAREN, 97};
static const int goTab_row_56[] = {5,TK_LPAREN, 23, TK_NUM, 25, TK_ID, 22, TK_STRING, 21, PRI, 98};
static const int goTab_row_57[] = {7,TK_REAL, 101, TK_ARRAY, 104, TK_INTEGER, 102, TK_ID, 99, TK_STRING, 100, TYPE, 105, STANDTYPE, 103};
static const int goTab_row_58[] = {1,TK_ASSIGN, 106};
static const int goTab_row_59[] = {2,TK_LPAREN, 107, ARGLIST, 108};
static const int goTab_row_61[] = {1,TK_ASSIGN, 109};
static const int goTab_row_62[] = {1,TK_RETURN, 110};
static const int goTab_row_63[] = {1,TK_PRINT, 111};
static const int goTab_row_64[] = {1,TK_WHILE, 112};
static const int goTab_row_65[] = {1,TK_END, 113};
static const int goTab_row_66[] = {1,TK_SEMI, 114};
static const int goTab_row_67[] = {1,#, 115};
static const int goTab_row_76[] = {1,TK_IF, 116};
static const int goTab_row_78[] = {2,TK_RPAREN, 117, TK_COMMA, 40};
static const int goTab_row_79[] = {6,TK_LPAREN, 23, TK_NUM, 25, TK_ID, 22, TK_STRING, 21, PRI, 52, POST, 118};
static const int goTab_row_80[] = {1,TK_PERIOD, 119};
static const int goTab_row_81[] = {1,TK_LSQB, 120};
static const int goTab_row_82[] = {1,TK_LPAREN, 121};
static const int goTab_row_83[] = {1,TK_MOD, 122};
static const int goTab_row_84[] = {1,TK_AND, 123};
static const int goTab_row_85[] = {1,TK_DIV, 124};
static const int goTab_row_86[] = {1,TK_MUL, 125};
static const int goTab_row_87[] = {8,TK_LPAREN, 23, TK_MINUS, 50, TK_NUM, 25, TK_ID, 22, TK_STRING, 21, PRI, 52, FACT, 126, POST, 49};
static const int goTab_row_88[] = {1,TK_OR, 127};
static const int goTab_row_89[] = {1,TK_MINUS, 128};
static const int goTab_row_90[] = {1,TK_PLUS, 129};
static const int goTab_row_91[] = {1,TK_NEQ, 130};
static const int goTab_row_92[] = {1,TK_EQU, 131};
static const int goTab_row_93[] = {1,TK_GT, 132};
static const int goTab_row_94[] = {1,TK_LT, 133};
static const int goTab_row_95[] = {9,TK_LPAREN, 23, TK_MINUS, 50, TK_NUM, 25, TK_ID, 22, TK_STRING, 21, PRI, 52, TERM, 134, FACT, 51, POST, 49};
static const int goTab_row_96[] = {10,TK_LPAREN, 23, TK_MINUS, 50, TK_NUM, 25, TK_ID, 22, TK_STRING, 21, PRI, 52, SIMPEXPR, 135, TERM, 53, FACT, 51, POST, 49};
static const int goTab_row_97[] = {1,TK_RPAREN, 136};
static const int goTab_row_99[] = {1,TK_ID, 137};
static const int goTab_row_100[] = {1,TK_STRING, 138};
static const int goTab_row_101[] = {1,TK_REAL, 139};
static const int goTab_row_102[] = {1,TK_INTEGER, 140};
static const int goTab_row_104[] = {1,TK_ARRAY, 141};
static const int goTab_row_105[] = {1,TK_SEMI, 142};
static const int goTab_row_106[] = {1,TK_RECORD, 143};
static const int goTab_row_107[] = {1,TK_LPAREN, 144};
static const int goTab_row_108[] = {1,TK_COLON, 145};
static const int goTab_row_109[] = {1,TK_ASSIGN, 146};
static const int goTab_row_110[] = {11,TK_LPAREN, 23, TK_MINUS, 50, TK_NUM, 25, TK_ID, 22, TK_STRING, 21, PRI, 52, EXPR, 147, SIMPEXPR, 54, TERM, 53, FACT, 51, POST, 49};
static const int goTab_row_111[] = {11,TK_LPAREN, 23, TK_MINUS, 50, TK_NUM, 25, TK_ID, 22, TK_STRING, 21, PRI, 52, EXPR, 148, SIMPEXPR, 54, TERM, 53, FACT, 51, POST, 49};
static const int goTab_row_112[] = {11,TK_LPAREN, 23, TK_MINUS, 50, TK_NUM, 25, TK_ID, 22, TK_STRING, 21, PRI, 52, EXPR, 149, SIMPEXPR, 54, TERM, 53, FACT, 51, POST, 49};
static const int goTab_row_113[] = {1,TK_END, 150};
static const int goTab_row_114[] = {1,TK_SEMI, 151};
static const int goTab_row_116[] = {11,TK_LPAREN, 23, TK_MINUS, 50, TK_NUM, 25, TK_ID, 22, TK_STRING, 21, PRI, 52, EXPR, 152, SIMPEXPR, 54, TERM, 53, FACT, 51, POST, 49};
static const int goTab_row_117[] = {1,TK_RPAREN, 153};
static const int goTab_row_119[] = {5,TK_LPAREN, 23, TK_NUM, 25, TK_ID, 22, TK_STRING, 21, PRI, 154};
static const int goTab_row_120[] = {11,TK_LPAREN, 23, TK_MINUS, 50, TK_NUM, 25, TK_ID, 22, TK_STRING, 21, PRI, 52, EXPR, 155, SIMPEXPR, 54, TERM, 53, FACT, 51, POST, 49};
static const int goTab_row_121[] = {12,TK_LPAREN, 23, TK_MINUS, 50, TK_NUM, 25, TK_ID, 22, TK_STRING, 21, PRI, 52, EXPR, 156, EXPRLIST, 157, SIMPEXPR, 54, TERM, 53, FACT, 51, POST, 49};
static const int goTab_row_135[] = {4,TK_OR, 88, TK_PLUS, 90, TK_MINUS, 89, ADDOP, 95};
static const int goTab_row_141[] = {1,TK_LSQB, 158};
static const int goTab_row_142[] = {1,TK_SEMI, 159};
static const int goTab_row_143[] = {1,TK_RECORD, 160};
static const int goTab_row_144[] = {7,TK_LPAREN, 23, TK_NUM, 25, TK_ID, 22, TK_STRING, 21, IDENTLIST, 161, PRI, 24, PARAMLIST, 162};
static const int goTab_row_145[] = {1,TK_COLON, 163};
static const int goTab_row_146[] = {11,TK_LPAREN, 23, TK_MINUS, 50, TK_NUM, 25, TK_ID, 22, TK_STRING, 21, PRI, 52, EXPR, 164, SIMPEXPR, 54, TERM, 53, FACT, 51, POST, 49};
static const int goTab_row_149[] = {1,TK_DO, 165};
static const int goTab_row_151[] = {24,TK_IF, 76, TK_BEGIN, 32, TK_WHILE, 64, TK_RETURN, 62, TK_PRINT, 63, TK_LPAREN, 23, TK_MINUS, 50, TK_NUM, 25, TK_ID, 22, TK_STRING, 21, COMPOUNDSTMT, 71, PRI, 52, EXPR, 61, STMT, 166, EXPRSTMT, 70, IFSTMT, 75, WHILESTMT, 68, PRINTSTMT, 72, RETSTMT, 73, AEXPR, 74, SIMPEXPR, 54, TERM, 53, FACT, 51, POST, 49};
static const int goTab_row_152[] = {1,TK_THEN, 167};
static const int goTab_row_153[] = {1,TK_SEMI, 168};
static const int goTab_row_155[] = {1,TK_RSQB, 169};
static const int goTab_row_157[] = {2,TK_RPAREN, 171, TK_COMMA, 170};
static const int goTab_row_158[] = {1,TK_LSQB, 172};
static const int goTab_row_160[] = {3,TK_ID, 173, FIELDLIST, 175, RECFIELD, 174};
static const int goTab_row_161[] = {2,TK_COLON, 176, TK_COMMA, 40};
static const int goTab_row_162[] = {2,TK_RPAREN, 178, TK_SEMI, 177};
static const int goTab_row_163[] = {7,TK_REAL, 101, TK_ARRAY, 104, TK_INTEGER, 102, TK_ID, 99, TK_STRING, 100, TYPE, 179, STANDTYPE, 103};
static const int goTab_row_165[] = {1,TK_DO, 180};
static const int goTab_row_167[] = {1,TK_THEN, 181};
static const int goTab_row_168[] = {1,TK_SEMI, 182};
static const int goTab_row_169[] = {1,TK_RSQB, 183};
static const int goTab_row_170[] = {1,TK_COMMA, 184};
static const int goTab_row_171[] = {1,TK_RPAREN, 185};
static const int goTab_row_172[] = {11,TK_LPAREN, 23, TK_MINUS, 50, TK_NUM, 25, TK_ID, 22, TK_STRING, 21, PRI, 52, EXPR, 186, SIMPEXPR, 54, TERM, 53, FACT, 51, POST, 49};
static const int goTab_row_173[] = {1,TK_ID, 187};
static const int goTab_row_174[] = {3,TK_ID, 173, FIELDLIST, 188, RECFIELD, 174};
static const int goTab_row_175[] = {1,TK_END, 189};
static const int goTab_row_176[] = {1,TK_COLON, 190};
static const int goTab_row_177[] = {1,TK_SEMI, 191};
static const int goTab_row_178[] = {1,TK_RPAREN, 192};
static const int goTab_row_179[] = {1,TK_SEMI, 193};
static const int goTab_row_180[] = {24,TK_IF, 76, TK_BEGIN, 32, TK_WHILE, 64, TK_RETURN, 62, TK_PRINT, 63, TK_LPAREN, 23, TK_MINUS, 50, TK_NUM, 25, TK_ID, 22, TK_STRING, 21, COMPOUNDSTMT, 71, PRI, 52, EXPR, 61, STMT, 194, EXPRSTMT, 70, IFSTMT, 75, WHILESTMT, 68, PRINTSTMT, 72, RETSTMT, 73, AEXPR, 74, SIMPEXPR, 54, TERM, 53, FACT, 51, POST, 49};
static const int goTab_row_181[] = {24,TK_IF, 76, TK_BEGIN, 32, TK_WHILE, 64, TK_RETURN, 62, TK_PRINT, 63, TK_LPAREN, 23, TK_MINUS, 50, TK_NUM, 25, TK_ID, 22, TK_STRING, 21, COMPOUNDSTMT, 71, PRI, 52, EXPR, 61, STMT, 195, EXPRSTMT, 70, IFSTMT, 75, WHILESTMT, 68, PRINTSTMT, 72, RETSTMT, 73, AEXPR, 74, SIMPEXPR, 54, TERM, 53, FACT, 51, POST, 49};
static const int goTab_row_184[] = {11,TK_LPAREN, 23, TK_MINUS, 50, TK_NUM, 25, TK_ID, 22, TK_STRING, 21, PRI, 52, EXPR, 196, SIMPEXPR, 54, TERM, 53, FACT, 51, POST, 49};
static const int goTab_row_186[] = {1,TK_RANGE, 197};
static const int goTab_row_187[] = {1,TK_COLON, 198};
static const int goTab_row_189[] = {1,TK_END, 199};
static const int goTab_row_190[] = {7,TK_REAL, 101, TK_ARRAY, 104, TK_INTEGER, 102, TK_ID, 99, TK_STRING, 100, TYPE, 200, STANDTYPE, 103};
static const int goTab_row_191[] = {6,TK_LPAREN, 23, TK_NUM, 25, TK_ID, 22, TK_STRING, 21, IDENTLIST, 201, PRI, 24};
static const int goTab_row_193[] = {1,TK_SEMI, 202};
static const int goTab_row_195[] = {1,TK_ELSE, 203};
static const int goTab_row_197[] = {1,TK_RANGE, 204};
static const int goTab_row_198[] = {1,TK_COLON, 205};
static const int goTab_row_199[] = {1,TK_SEMI, 206};
static const int goTab_row_201[] = {2,TK_COLON, 207, TK_COMMA, 40};
static const int goTab_row_203[] = {1,TK_ELSE, 208};
static const int goTab_row_204[] = {11,TK_LPAREN, 23, TK_MINUS, 50, TK_NUM, 25, TK_ID, 22, TK_STRING, 21, PRI, 52, EXPR, 209, SIMPEXPR, 54, TERM, 53, FACT, 51, POST, 49};
static const int goTab_row_205[] = {7,TK_REAL, 101, TK_ARRAY, 104, TK_INTEGER, 102, TK_ID, 99, TK_STRING, 100, TYPE, 210, STANDTYPE, 103};
static const int goTab_row_206[] = {1,TK_SEMI, 211};
static const int goTab_row_207[] = {1,TK_COLON, 212};
static const int goTab_row_208[] = {24,TK_IF, 76, TK_BEGIN, 32, TK_WHILE, 64, TK_RETURN, 62, TK_PRINT, 63, TK_LPAREN, 23, TK_MINUS, 50, TK_NUM, 25, TK_ID, 22, TK_STRING, 21, COMPOUNDSTMT, 71, PRI, 52, EXPR, 61, STMT, 213, EXPRSTMT, 70, IFSTMT, 75, WHILESTMT, 68, PRINTSTMT, 72, RETSTMT, 73, AEXPR, 74, SIMPEXPR, 54, TERM, 53, FACT, 51, POST, 49};
static const int goTab_row_209[] = {1,TK_RSQB, 214};
static const int goTab_row_210[] = {1,TK_SEMI, 215};
static const int goTab_row_212[] = {7,TK_REAL, 101, TK_ARRAY, 104, TK_INTEGER, 102, TK_ID, 99, TK_STRING, 100, TYPE, 216, STANDTYPE, 103};
static const int goTab_row_214[] = {1,TK_RSQB, 217};
static const int goTab_row_215[] = {1,TK_SEMI, 218};
static const int goTab_row_217[] = {1,TK_OF, 219};
static const int goTab_row_219[] = {1,TK_OF, 220};
static const int goTab_row_220[] = {5,TK_REAL, 101, TK_INTEGER, 102, TK_ID, 99, TK_STRING, 100, STANDTYPE, 221};

static const int *goTab[] = {
	 goTab_row_0, 	 goTab_row_1, 	 goTab_row_2, 	 NULL, 	 goTab_row_4, 
	 goTab_row_5, 	 goTab_row_6, 	 goTab_row_7, 	 goTab_row_8, 	 goTab_row_9, 
	 goTab_row_10, 	 goTab_row_11, 	 NULL, 	 goTab_row_13, 	 NULL, 
	 goTab_row_15, 	 goTab_row_16, 	 goTab_row_17, 	 goTab_row_18, 	 goTab_row_19, 
	 goTab_row_20, 	 goTab_row_21, 	 goTab_row_22, 	 goTab_row_23, 	 NULL, 
	 goTab_row_25, 	 goTab_row_26, 	 goTab_row_27, 	 goTab_row_28, 	 goTab_row_29, 
	 NULL, 	 goTab_row_31, 	 goTab_row_32, 	 goTab_row_33, 	 NULL, 
	 goTab_row_35, 	 NULL, 	 NULL, 	 goTab_row_38, 	 NULL, 
	 goTab_row_40, 	 goTab_row_41, 	 goTab_row_42, 	 goTab_row_43, 	 NULL, 
	 goTab_row_45, 	 goTab_row_46, 	 goTab_row_47, 	 goTab_row_48, 	 NULL, 
	 goTab_row_50, 	 NULL, 	 goTab_row_52, 	 goTab_row_53, 	 goTab_row_54, 
	 goTab_row_55, 	 goTab_row_56, 	 goTab_row_57, 	 goTab_row_58, 	 goTab_row_59, 
	 NULL, 	 goTab_row_61, 	 goTab_row_62, 	 goTab_row_63, 	 goTab_row_64, 
	 goTab_row_65, 	 goTab_row_66, 	 goTab_row_67, 	 NULL, 	 NULL, 
	 NULL, 	 NULL, 	 NULL, 	 NULL, 	 NULL, 
	 NULL, 	 goTab_row_76, 	 NULL, 	 goTab_row_78, 	 goTab_row_79, 
	 goTab_row_80, 	 goTab_row_81, 	 goTab_row_82, 	 goTab_row_83, 	 goTab_row_84, 
	 goTab_row_85, 	 goTab_row_86, 	 goTab_row_87, 	 goTab_row_88, 	 goTab_row_89, 
	 goTab_row_90, 	 goTab_row_91, 	 goTab_row_92, 	 goTab_row_93, 	 goTab_row_94, 
	 goTab_row_95, 	 goTab_row_96, 	 goTab_row_97, 	 NULL, 	 goTab_row_99, 
	 goTab_row_100, 	 goTab_row_101, 	 goTab_row_102, 	 NULL, 	 goTab_row_104, 
	 goTab_row_105, 	 goTab_row_106, 	 goTab_row_107, 	 goTab_row_108, 	 goTab_row_109, 
	 goTab_row_110, 	 goTab_row_111, 	 goTab_row_112, 	 goTab_row_113, 	 goTab_row_114, 
	 NULL, 	 goTab_row_116, 	 goTab_row_117, 	 NULL, 	 goTab_row_119, 
	 goTab_row_120, 	 goTab_row_121, 	 NULL, 	 NULL, 	 NULL, 
	 NULL, 	 NULL, 	 NULL, 	 NULL, 	 NULL, 
	 NULL, 	 NULL, 	 NULL, 	 NULL, 	 goTab_row_53, 
	 goTab_row_135, 	 NULL, 	 NULL, 	 NULL, 	 NULL, 
	 NULL, 	 goTab_row_141, 	 goTab_row_142, 	 goTab_row_143, 	 goTab_row_144, 
	 goTab_row_145, 	 goTab_row_146, 	 NULL, 	 NULL, 	 goTab_row_149, 
	 NULL, 	 goTab_row_151, 	 goTab_row_152, 	 goTab_row_153, 	 NULL, 
	 goTab_row_155, 	 NULL, 	 goTab_row_157, 	 goTab_row_158, 	 NULL, 
	 goTab_row_160, 	 goTab_row_161, 	 goTab_row_162, 	 goTab_row_163, 	 NULL, 
	 goTab_row_165, 	 NULL, 	 goTab_row_167, 	 goTab_row_168, 	 goTab_row_169, 
	 goTab_row_170, 	 goTab_row_171, 	 goTab_row_172, 	 goTab_row_173, 	 goTab_row_174, 
	 goTab_row_175, 	 goTab_row_176, 	 goTab_row_177, 	 goTab_row_178, 	 goTab_row_179, 
	 goTab_row_180, 	 goTab_row_181, 	 NULL, 	 NULL, 	 goTab_row_184, 
	 NULL, 	 goTab_row_186, 	 goTab_row_187, 	 NULL, 	 goTab_row_189, 
	 goTab_row_190, 	 goTab_row_191, 	 NULL, 	 goTab_row_193, 	 NULL, 
	 goTab_row_195, 	 NULL, 	 goTab_row_197, 	 goTab_row_198, 	 goTab_row_199, 
	 NULL, 	 goTab_row_201, 	 NULL, 	 goTab_row_203, 	 goTab_row_204, 
	 goTab_row_205, 	 goTab_row_206, 	 goTab_row_207, 	 goTab_row_208, 	 goTab_row_209, 
	 goTab_row_210, 	 NULL, 	 goTab_row_212, 	 NULL, 	 goTab_row_214, 
	 goTab_row_215, 	 NULL, 	 goTab_row_217, 	 NULL, 	 goTab_row_219, 
	 goTab_row_220
};

static const int actTab_row_0[] = {1,TK_PROGRAM, 1};
static const int actTab_row_1[] = {1,TK_PROGRAM, 4};
static const int actTab_row_2[] = {3,TK_VAR, 5, TK_TYPE, 7, #, 6};
static const int actTab_row_3[] = {1,DOLLARACCEPT, 0};
static const int actTab_row_4[] = {1,TK_ID, 10};
static const int actTab_row_5[] = {1,TK_VAR, 11};
static const int actTab_row_6[] = {1,#, 12};
static const int actTab_row_7[] = {1,TK_TYPE, 13};
static const int actTab_row_9[] = {2,TK_DEF, 15, #, 17};
static const int actTab_row_10[] = {1,TK_ID, 20};
static const int actTab_row_11[] = {4,TK_LPAREN, 23, TK_NUM, 25, TK_ID, 22, TK_STRING, 21};
static const int actTab_row_13[] = {1,TK_ID, 27};
static const int actTab_row_15[] = {1,TK_DEF, 28};
static const int actTab_row_17[] = {1,#, 30};
static const int actTab_row_18[] = {1,TK_SEMI, 31};
static const int actTab_row_19[] = {1,TK_BEGIN, 32};
static const int actTab_row_20[] = {1,TK_LPAREN, 35};
static const int actTab_row_21[] = {1,TK_STRING, 36};
static const int actTab_row_22[] = {1,TK_ID, 37};
static const int actTab_row_23[] = {1,TK_LPAREN, 38};
static const int actTab_row_24[] = {3,TK_RPAREN, -6, TK_COLON, -6, TK_COMMA, -6};
static const int actTab_row_25[] = {1,TK_NUM, 39};
static const int actTab_row_26[] = {2,TK_COLON, 41, TK_COMMA, 40};
static const int actTab_row_27[] = {1,TK_ID, 42};
static const int actTab_row_28[] = {1,TK_ID, 43};
static const int actTab_row_31[] = {1,TK_SEMI, 45};
static const int actTab_row_32[] = {1,TK_BEGIN, 46};
static const int actTab_row_33[] = {1,TK_PERIOD, 47};
static const int actTab_row_34[] = {1,TK_EOI, -2};
static const int actTab_row_35[] = {1,TK_LPAREN, 48};
static const int actTab_row_36[] = {14,TK_DO, -61, TK_END, -61, TK_ELSE, -61, TK_THEN, -61, TK_LPAREN, -61, TK_RPAREN, -61, TK_LSQB, -61, TK_RSQB, -61, TK_SEMI, -61, TK_COLON, -61, TK_ASSIGN, -61, TK_COMMA, -61, TK_RANGE, -61, TK_PERIOD, -61};
static const int actTab_row_37[] = {14,TK_DO, -59, TK_END, -59, TK_ELSE, -59, TK_THEN, -59, TK_LPAREN, -59, TK_RPAREN, -59, TK_LSQB, -59, TK_RSQB, -59, TK_SEMI, -59, TK_COLON, -59, TK_ASSIGN, -59, TK_COMMA, -59, TK_RANGE, -59, TK_PERIOD, -59};
static const int actTab_row_38[] = {5,TK_LPAREN, 23, TK_MINUS, 50, TK_NUM, 25, TK_ID, 22, TK_STRING, 21};
static const int actTab_row_39[] = {14,TK_DO, -60, TK_END, -60, TK_ELSE, -60, TK_THEN, -60, TK_LPAREN, -60, TK_RPAREN, -60, TK_LSQB, -60, TK_RSQB, -60, TK_SEMI, -60, TK_COLON, -60, TK_ASSIGN, -60, TK_COMMA, -60, TK_RANGE, -60, TK_PERIOD, -60};
static const int actTab_row_40[] = {1,TK_COMMA, 56};
static const int actTab_row_41[] = {1,TK_COLON, 57};
static const int actTab_row_42[] = {1,TK_ASSIGN, 58};
static const int actTab_row_43[] = {1,TK_ID, 59};
static const int actTab_row_44[] = {1,TK_SEMI, -22};
static const int actTab_row_46[] = {11,TK_IF, 76, TK_BEGIN, 32, TK_WHILE, 64, TK_RETURN, 62, TK_PRINT, 63, TK_LPAREN, 23, TK_MINUS, 50, TK_NUM, 25, TK_ID, 22, TK_STRING, 21, #, 67};
static const int actTab_row_47[] = {1,TK_PERIOD, 77};
static const int actTab_row_49[] = {10,TK_DO, -54, TK_END, -54, TK_ELSE, -54, TK_THEN, -54, TK_RPAREN, -54, TK_RSQB, -54, TK_SEMI, -54, TK_ASSIGN, -54, TK_COMMA, -54, TK_RANGE, -54};
static const int actTab_row_50[] = {1,TK_MINUS, 79};
static const int actTab_row_51[] = {10,TK_DO, -52, TK_END, -52, TK_ELSE, -52, TK_THEN, -52, TK_RPAREN, -52, TK_RSQB, -52, TK_SEMI, -52, TK_ASSIGN, -52, TK_COMMA, -52, TK_RANGE, -52};
static const int actTab_row_52[] = {13,TK_DO, -58, TK_END, -58, TK_ELSE, -58, TK_THEN, -58, TK_LPAREN, 82, TK_RPAREN, -58, TK_LSQB, 81, TK_RSQB, -58, TK_SEMI, -58, TK_ASSIGN, -58, TK_COMMA, -58, TK_RANGE, -58, TK_PERIOD, 80};
static const int actTab_row_53[] = {14,TK_DO, -50, TK_AND, 84, TK_END, -50, TK_MOD, 83, TK_ELSE, -50, TK_THEN, -50, TK_RPAREN, -50, TK_RSQB, -50, TK_MUL, 86, TK_DIV, 85, TK_SEMI, -50, TK_ASSIGN, -50, TK_COMMA, -50, TK_RANGE, -50};
static const int actTab_row_54[] = {17,TK_DO, -48, TK_OR, 88, TK_END, -48, TK_ELSE, -48, TK_THEN, -48, TK_RPAREN, -48, TK_RSQB, -48, TK_PLUS, 90, TK_MINUS, 89, TK_LT, 94, TK_GT, 93, TK_SEMI, -48, TK_ASSIGN, -48, TK_EQU, 92, TK_NEQ, 91, TK_COMMA, -48, TK_RANGE, -48};
static const int actTab_row_55[] = {1,TK_RPAREN, 97};
static const int actTab_row_57[] = {5,TK_REAL, 101, TK_ARRAY, 104, TK_INTEGER, 102, TK_ID, 99, TK_STRING, 100};
static const int actTab_row_58[] = {1,TK_ASSIGN, 106};
static const int actTab_row_59[] = {1,TK_LPAREN, 107};
static const int actTab_row_61[] = {4,TK_END, -46, TK_ELSE, -46, TK_SEMI, -46, TK_ASSIGN, 109};
static const int actTab_row_62[] = {1,TK_RETURN, 110};
static const int actTab_row_63[] = {1,TK_PRINT, 111};
static const int actTab_row_64[] = {1,TK_WHILE, 112};
static const int actTab_row_65[] = {1,TK_END, 113};
static const int actTab_row_66[] = {2,TK_END, -28, TK_SEMI, 114};
static const int actTab_row_67[] = {1,#, 115};
static const int actTab_row_68[] = {3,TK_END, -35, TK_ELSE, -35, TK_SEMI, -35};
static const int actTab_row_69[] = {2,TK_END, -31, TK_SEMI, -31};
static const int actTab_row_70[] = {3,TK_END, -32, TK_ELSE, -32, TK_SEMI, -32};
static const int actTab_row_71[] = {3,TK_END, -33, TK_ELSE, -33, TK_SEMI, -33};
static const int actTab_row_72[] = {3,TK_END, -36, TK_ELSE, -36, TK_SEMI, -36};
static const int actTab_row_73[] = {3,TK_END, -37, TK_ELSE, -37, TK_SEMI, -37};
static const int actTab_row_74[] = {3,TK_END, -38, TK_ELSE, -38, TK_SEMI, -38};
static const int actTab_row_75[] = {3,TK_END, -34, TK_ELSE, -34, TK_SEMI, -34};
static const int actTab_row_76[] = {1,TK_IF, 116};
static const int actTab_row_77[] = {1,TK_EOI, -4};
static const int actTab_row_78[] = {2,TK_RPAREN, 117, TK_COMMA, 40};
static const int actTab_row_80[] = {1,TK_PERIOD, 119};
static const int actTab_row_81[] = {1,TK_LSQB, 120};
static const int actTab_row_82[] = {1,TK_LPAREN, 121};
static const int actTab_row_83[] = {1,TK_MOD, 122};
static const int actTab_row_84[] = {1,TK_AND, 123};
static const int actTab_row_85[] = {1,TK_DIV, 124};
static const int actTab_row_86[] = {1,TK_MUL, 125};
static const int actTab_row_88[] = {1,TK_OR, 127};
static const int actTab_row_89[] = {1,TK_MINUS, 128};
static const int actTab_row_90[] = {1,TK_PLUS, 129};
static const int actTab_row_91[] = {1,TK_NEQ, 130};
static const int actTab_row_92[] = {1,TK_EQU, 131};
static const int actTab_row_93[] = {1,TK_GT, 132};
static const int actTab_row_94[] = {1,TK_LT, 133};
static const int actTab_row_97[] = {1,TK_RPAREN, 136};
static const int actTab_row_98[] = {3,TK_RPAREN, -5, TK_COLON, -5, TK_COMMA, -5};
static const int actTab_row_99[] = {1,TK_ID, 137};
static const int actTab_row_100[] = {1,TK_STRING, 138};
static const int actTab_row_101[] = {1,TK_REAL, 139};
static const int actTab_row_102[] = {1,TK_INTEGER, 140};
static const int actTab_row_103[] = {2,TK_RPAREN, -11, TK_SEMI, -11};
static const int actTab_row_104[] = {1,TK_ARRAY, 141};
static const int actTab_row_105[] = {1,TK_SEMI, 142};
static const int actTab_row_106[] = {1,TK_RECORD, 143};
static const int actTab_row_107[] = {1,TK_LPAREN, 144};
static const int actTab_row_108[] = {1,TK_COLON, 145};
static const int actTab_row_109[] = {1,TK_ASSIGN, 146};
static const int actTab_row_113[] = {1,TK_END, 150};
static const int actTab_row_114[] = {1,TK_SEMI, 151};
static const int actTab_row_115[] = {1,TK_END, -29};
static const int actTab_row_117[] = {1,TK_RPAREN, 153};
static const int actTab_row_118[] = {10,TK_DO, -53, TK_END, -53, TK_ELSE, -53, TK_THEN, -53, TK_RPAREN, -53, TK_RSQB, -53, TK_SEMI, -53, TK_ASSIGN, -53, TK_COMMA, -53, TK_RANGE, -53};
static const int actTab_row_126[] = {10,TK_DO, -51, TK_END, -51, TK_ELSE, -51, TK_THEN, -51, TK_RPAREN, -51, TK_RSQB, -51, TK_SEMI, -51, TK_ASSIGN, -51, TK_COMMA, -51, TK_RANGE, -51};
static const int actTab_row_134[] = {14,TK_DO, -49, TK_AND, 84, TK_END, -49, TK_MOD, 83, TK_ELSE, -49, TK_THEN, -49, TK_RPAREN, -49, TK_RSQB, -49, TK_MUL, 86, TK_DIV, 85, TK_SEMI, -49, TK_ASSIGN, -49, TK_COMMA, -49, TK_RANGE, -49};
static const int actTab_row_135[] = {13,TK_DO, -47, TK_OR, 88, TK_END, -47, TK_ELSE, -47, TK_THEN, -47, TK_RPAREN, -47, TK_RSQB, -47, TK_PLUS, 90, TK_MINUS, 89, TK_SEMI, -47, TK_ASSIGN, -47, TK_COMMA, -47, TK_RANGE, -47};
static const int actTab_row_136[] = {14,TK_DO, -62, TK_END, -62, TK_ELSE, -62, TK_THEN, -62, TK_LPAREN, -62, TK_RPAREN, -62, TK_LSQB, -62, TK_RSQB, -62, TK_SEMI, -62, TK_COLON, -62, TK_ASSIGN, -62, TK_COMMA, -62, TK_RANGE, -62, TK_PERIOD, -62};
static const int actTab_row_137[] = {2,TK_RPAREN, -19, TK_SEMI, -19};
static const int actTab_row_138[] = {2,TK_RPAREN, -18, TK_SEMI, -18};
static const int actTab_row_139[] = {2,TK_RPAREN, -17, TK_SEMI, -17};
static const int actTab_row_140[] = {2,TK_RPAREN, -16, TK_SEMI, -16};
static const int actTab_row_141[] = {1,TK_LSQB, 158};
static const int actTab_row_142[] = {1,TK_SEMI, 159};
static const int actTab_row_143[] = {1,TK_RECORD, 160};
static const int actTab_row_145[] = {1,TK_COLON, 163};
static const int actTab_row_147[] = {3,TK_END, -42, TK_ELSE, -42, TK_SEMI, -42};
static const int actTab_row_148[] = {3,TK_END, -41, TK_ELSE, -41, TK_SEMI, -41};
static const int actTab_row_149[] = {1,TK_DO, 165};
static const int actTab_row_150[] = {4,TK_END, -27, TK_ELSE, -27, TK_SEMI, -27, TK_PERIOD, -27};
static const int actTab_row_151[] = {10,TK_IF, 76, TK_BEGIN, 32, TK_WHILE, 64, TK_RETURN, 62, TK_PRINT, 63, TK_LPAREN, 23, TK_MINUS, 50, TK_NUM, 25, TK_ID, 22, TK_STRING, 21};
static const int actTab_row_152[] = {1,TK_THEN, 167};
static const int actTab_row_153[] = {1,TK_SEMI, 168};
static const int actTab_row_154[] = {10,TK_DO, -57, TK_END, -57, TK_ELSE, -57, TK_THEN, -57, TK_RPAREN, -57, TK_RSQB, -57, TK_SEMI, -57, TK_ASSIGN, -57, TK_COMMA, -57, TK_RANGE, -57};
static const int actTab_row_155[] = {1,TK_RSQB, 169};
static const int actTab_row_156[] = {2,TK_RPAREN, -44, TK_COMMA, -44};
static const int actTab_row_157[] = {2,TK_RPAREN, 171, TK_COMMA, 170};
static const int actTab_row_158[] = {1,TK_LSQB, 172};
static const int actTab_row_160[] = {1,TK_ID, 173};
static const int actTab_row_161[] = {2,TK_COLON, 176, TK_COMMA, 40};
static const int actTab_row_162[] = {2,TK_RPAREN, 178, TK_SEMI, 177};
static const int actTab_row_164[] = {3,TK_END, -45, TK_ELSE, -45, TK_SEMI, -45};
static const int actTab_row_165[] = {1,TK_DO, 180};
static const int actTab_row_166[] = {2,TK_END, -30, TK_SEMI, -30};
static const int actTab_row_167[] = {1,TK_THEN, 181};
static const int actTab_row_168[] = {1,TK_SEMI, 182};
static const int actTab_row_169[] = {1,TK_RSQB, 183};
static const int actTab_row_170[] = {1,TK_COMMA, 184};
static const int actTab_row_171[] = {1,TK_RPAREN, 185};
static const int actTab_row_173[] = {1,TK_ID, 187};
static const int actTab_row_174[] = {2,TK_END, -14, TK_ID, 173};
static const int actTab_row_175[] = {1,TK_END, 189};
static const int actTab_row_176[] = {1,TK_COLON, 190};
static const int actTab_row_177[] = {1,TK_SEMI, 191};
static const int actTab_row_178[] = {1,TK_RPAREN, 192};
static const int actTab_row_179[] = {1,TK_SEMI, 193};
static const int actTab_row_183[] = {10,TK_DO, -56, TK_END, -56, TK_ELSE, -56, TK_THEN, -56, TK_RPAREN, -56, TK_RSQB, -56, TK_SEMI, -56, TK_ASSIGN, -56, TK_COMMA, -56, TK_RANGE, -56};
static const int actTab_row_185[] = {10,TK_DO, -55, TK_END, -55, TK_ELSE, -55, TK_THEN, -55, TK_RPAREN, -55, TK_RSQB, -55, TK_SEMI, -55, TK_ASSIGN, -55, TK_COMMA, -55, TK_RANGE, -55};
static const int actTab_row_186[] = {1,TK_RANGE, 197};
static const int actTab_row_187[] = {1,TK_COLON, 198};
static const int actTab_row_188[] = {1,TK_END, -13};
static const int actTab_row_189[] = {1,TK_END, 199};
static const int actTab_row_192[] = {1,TK_COLON, -24};
static const int actTab_row_193[] = {1,TK_SEMI, 202};
static const int actTab_row_194[] = {3,TK_END, -40, TK_ELSE, -40, TK_SEMI, -40};
static const int actTab_row_195[] = {1,TK_ELSE, 203};
static const int actTab_row_196[] = {2,TK_RPAREN, -43, TK_COMMA, -43};
static const int actTab_row_197[] = {1,TK_RANGE, 204};
static const int actTab_row_198[] = {1,TK_COLON, 205};
static const int actTab_row_199[] = {1,TK_SEMI, 206};
static const int actTab_row_200[] = {2,TK_RPAREN, -26, TK_SEMI, -26};
static const int actTab_row_201[] = {2,TK_COLON, 207, TK_COMMA, 40};
static const int actTab_row_203[] = {1,TK_ELSE, 208};
static const int actTab_row_206[] = {1,TK_SEMI, 211};
static const int actTab_row_207[] = {1,TK_COLON, 212};
static const int actTab_row_209[] = {1,TK_RSQB, 214};
static const int actTab_row_210[] = {1,TK_SEMI, 215};
static const int actTab_row_213[] = {3,TK_END, -39, TK_ELSE, -39, TK_SEMI, -39};
static const int actTab_row_214[] = {1,TK_RSQB, 217};
static const int actTab_row_215[] = {1,TK_SEMI, 218};
static const int actTab_row_216[] = {2,TK_RPAREN, -25, TK_SEMI, -25};
static const int actTab_row_217[] = {1,TK_OF, 219};
static const int actTab_row_218[] = {1,TK_END, -15};
static const int actTab_row_219[] = {1,TK_OF, 220};
static const int actTab_row_220[] = {4,TK_REAL, 101, TK_INTEGER, 102, TK_ID, 99, TK_STRING, 100};
static const int actTab_row_221[] = {2,TK_RPAREN, -12, TK_SEMI, -12};

static const int *actTab[] = {
	 actTab_row_0, 	 actTab_row_1, 	 actTab_row_2, 	 actTab_row_3, 	 actTab_row_4, 
	 actTab_row_5, 	 actTab_row_6, 	 actTab_row_7, 	 actTab_row_2, 	 actTab_row_9, 
	 actTab_row_10, 	 actTab_row_11, 	 NULL, 	 actTab_row_13, 	 NULL, 
	 actTab_row_15, 	 actTab_row_2, 	 actTab_row_17, 	 actTab_row_18, 	 actTab_row_19, 
	 actTab_row_20, 	 actTab_row_21, 	 actTab_row_22, 	 actTab_row_23, 	 actTab_row_24, 
	 actTab_row_25, 	 actTab_row_26, 	 actTab_row_27, 	 actTab_row_28, 	 actTab_row_19, 
	 NULL, 	 actTab_row_31, 	 actTab_row_32, 	 actTab_row_33, 	 actTab_row_34, 
	 actTab_row_35, 	 actTab_row_36, 	 actTab_row_37, 	 actTab_row_38, 	 actTab_row_39, 
	 actTab_row_40, 	 actTab_row_41, 	 actTab_row_42, 	 actTab_row_43, 	 actTab_row_44, 
	 actTab_row_9, 	 actTab_row_46, 	 actTab_row_47, 	 actTab_row_11, 	 actTab_row_49, 
	 actTab_row_50, 	 actTab_row_51, 	 actTab_row_52, 	 actTab_row_53, 	 actTab_row_54, 
	 actTab_row_55, 	 actTab_row_11, 	 actTab_row_57, 	 actTab_row_58, 	 actTab_row_59, 
	 NULL, 	 actTab_row_61, 	 actTab_row_62, 	 actTab_row_63, 	 actTab_row_64, 
	 actTab_row_65, 	 actTab_row_66, 	 actTab_row_67, 	 actTab_row_68, 	 actTab_row_69, 
	 actTab_row_70, 	 actTab_row_71, 	 actTab_row_72, 	 actTab_row_73, 	 actTab_row_74, 
	 actTab_row_75, 	 actTab_row_76, 	 actTab_row_77, 	 actTab_row_78, 	 actTab_row_11, 
	 actTab_row_80, 	 actTab_row_81, 	 actTab_row_82, 	 actTab_row_83, 	 actTab_row_84, 
	 actTab_row_85, 	 actTab_row_86, 	 actTab_row_38, 	 actTab_row_88, 	 actTab_row_89, 
	 actTab_row_90, 	 actTab_row_91, 	 actTab_row_92, 	 actTab_row_93, 	 actTab_row_94, 
	 actTab_row_38, 	 actTab_row_38, 	 actTab_row_97, 	 actTab_row_98, 	 actTab_row_99, 
	 actTab_row_100, 	 actTab_row_101, 	 actTab_row_102, 	 actTab_row_103, 	 actTab_row_104, 
	 actTab_row_105, 	 actTab_row_106, 	 actTab_row_107, 	 actTab_row_108, 	 actTab_row_109, 
	 actTab_row_38, 	 actTab_row_38, 	 actTab_row_38, 	 actTab_row_113, 	 actTab_row_114, 
	 actTab_row_115, 	 actTab_row_38, 	 actTab_row_117, 	 actTab_row_118, 	 actTab_row_11, 
	 actTab_row_38, 	 actTab_row_38, 	 NULL, 	 NULL, 	 NULL, 
	 NULL, 	 actTab_row_126, 	 NULL, 	 NULL, 	 NULL, 
	 NULL, 	 NULL, 	 NULL, 	 NULL, 	 actTab_row_134, 
	 actTab_row_135, 	 actTab_row_136, 	 actTab_row_137, 	 actTab_row_138, 	 actTab_row_139, 
	 actTab_row_140, 	 actTab_row_141, 	 actTab_row_142, 	 actTab_row_143, 	 actTab_row_11, 
	 actTab_row_145, 	 actTab_row_38, 	 actTab_row_147, 	 actTab_row_148, 	 actTab_row_149, 
	 actTab_row_150, 	 actTab_row_151, 	 actTab_row_152, 	 actTab_row_153, 	 actTab_row_154, 
	 actTab_row_155, 	 actTab_row_156, 	 actTab_row_157, 	 actTab_row_158, 	 NULL, 
	 actTab_row_160, 	 actTab_row_161, 	 actTab_row_162, 	 actTab_row_57, 	 actTab_row_164, 
	 actTab_row_165, 	 actTab_row_166, 	 actTab_row_167, 	 actTab_row_168, 	 actTab_row_169, 
	 actTab_row_170, 	 actTab_row_171, 	 actTab_row_38, 	 actTab_row_173, 	 actTab_row_174, 
	 actTab_row_175, 	 actTab_row_176, 	 actTab_row_177, 	 actTab_row_178, 	 actTab_row_179, 
	 actTab_row_151, 	 actTab_row_151, 	 NULL, 	 actTab_row_183, 	 actTab_row_38, 
	 actTab_row_185, 	 actTab_row_186, 	 actTab_row_187, 	 actTab_row_188, 	 actTab_row_189, 
	 actTab_row_57, 	 actTab_row_11, 	 actTab_row_192, 	 actTab_row_193, 	 actTab_row_194, 
	 actTab_row_195, 	 actTab_row_196, 	 actTab_row_197, 	 actTab_row_198, 	 actTab_row_199, 
	 actTab_row_200, 	 actTab_row_201, 	 NULL, 	 actTab_row_203, 	 actTab_row_38, 
	 actTab_row_57, 	 actTab_row_206, 	 actTab_row_207, 	 actTab_row_151, 	 actTab_row_209, 
	 actTab_row_210, 	 NULL, 	 actTab_row_57, 	 actTab_row_213, 	 actTab_row_214, 
	 actTab_row_215, 	 actTab_row_216, 	 actTab_row_217, 	 actTab_row_218, 	 actTab_row_219, 
	 actTab_row_220, 	 actTab_row_221
};


static const map<string, function<AST*(vector<AST*>&)>> actions = {
	 {"binop",binop}, 
	 {"mkArray",mkArray}, 
	 {"mkBlock",mkBlock}, 
	 {"mkCall",mkCall}, 
	 {"mkFunc",mkFunc}, 
	 {"mkFuncHeader",mkFuncHeader}, 
	 {"mkId",mkId}, 
	 {"mkIf",mkIf}, 
	 {"mkLet",mkLet}, 
	 {"mkList",mkList}, 
	 {"mkNum",mkNum}, 
	 {"mkPrint",mkPrint}, 
	 {"mkProgram",mkProgram}, 
	 {"mkProgramHeader",mkProgramHeader}, 
	 {"mkRecField",mkRecField}, 
	 {"mkRecord",mkRecord}, 
	 {"mkRet",mkRet}, 
	 {"mkString",mkString}, 
	 {"mkSubscript",mkSubscript}, 
	 {"mkTypeExpr",mkTypeExpr}, 
	 {"mkWhile",mkWhile}, 
	 {"pass",pass}, 
	 {"through",through}, 
	 {"unary",unary}

};
