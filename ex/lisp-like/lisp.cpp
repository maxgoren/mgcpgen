#include <iostream>
#include "lexer.hpp"
#include "parse.hpp"
#include "lisp.hpp"
using namespace std;

int main(int argc, char* argv[]) {
	initParseTable();
	Lexer lexer;
	Parser parser(parseTable, terminalSymbols, nonTerminalSymbols);
	string buffer;
	StringBuffer* sb = new StringBuffer();
	SexprVisitor* sv = new SexprVisitor();
	while (true) {
		getline(cin, buffer);
		sb->init(buffer);
		auto tokens =  lexer.lex(sb);
		for (auto t :tokens ) {
			cout<<"("<<tokenStr[t.getSymbol()]<<", "<<t.getString()<<")"<<endl;
		}
		List* tt = (List*)parser.parse(tokens, "sexpr");
		sv->visit(tt);
	}
	return 0;
}