#include <fstream>
#include <iostream>
#include "src/buffer.hpp"
#include "src/cfg.hpp"
#include "src/ParserGenerator.hpp"
#include "ex/algol_like/lexer.hpp"
#include "ex/algol_like/parse.hpp"
using namespace std;


vector<string> split(string input, char delim) {
    vector<string> result;
    string buffer;
    for (char c : input) {
        if (c != delim) {
            buffer.push_back(c);
        } else {
            result.push_back(buffer);
            buffer.clear();
        }
    }
    result.push_back(buffer);
    return result;
}

Grammar readGrammarFile(string filename) {
    Grammar G;
    string buff;
    ifstream infile(filename, ios::in);
    if (!infile.is_open()) {
        cout<<"Error: Couldn't open '"<<filename<<"' - unknown file."<<endl;
        return G;
    }
    int rulenum = 1;
    while (infile.good()) {
        getline(infile, buff);
        vector<string> parts = split(buff, ' ');
        G.nonterminals.insert(parts[0]);
        ProductionSet ps = G.productions[parts[0]];
        SymbolString ss;
        G.terminals.insert(GOAL);
        G.terminals.insert(EPS);
        for (int i = 2; i < parts.size(); i++) {
            string s = parts[i];
            if (s[0] == 'T' && s[1] == 'K' && s[2] == '_') {
                G.terminals.insert(s);
            } else {
                G.nonterminals.insert(s);
            }
            ss.push_back(s);
        }
        ps.push_back(Production(rulenum++, parts[0], ss));
        G.productions[parts[0]] = ps;
    }
    return G; 
}

void interpreter(Grammar& G, ParseTable& table, string expr) {
    Lexer lexer;
    Parser parser(G, table);
    string inbuff = expr;
    StringBuffer* sb = new StringBuffer();
    do {
        sb->init(inbuff);
        vector<Token> tokens = lexer.lex(sb);
        AST* ast = parser.parse(tokens,"stmt-seq");
        if (ast != nullptr) ast->print(), cout<<ast->eval()<<endl;
        cout<<"\n -=> ";
        getline(cin, inbuff);
    } while (true);
}

void showUsage(string name) {
    cout<<"MGCPGen (c) 2026 MaxGCoding.com"<<endl;
    cout<<name<<" <grammar file>"<<endl;
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        showUsage(argv[0]);
        return 1;
    }
    Grammar G = readGrammarFile(argv[1]);
    ParserGenerator pg;
    ParseTable table = pg.generate(G, "stmt-seq");
    pg.tableGen.persist("test.gmpt", G);
    interpreter(G, table, "prev := 0; curr := 1; while (curr < 40) { next := prev + curr; print next; prev := curr; curr := next; };");
    return 0;
}