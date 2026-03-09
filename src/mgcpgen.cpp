#include <iostream>
#include "cfg.hpp"
#include "ParserGenerator.hpp"
using namespace std;

void showUsage(string name) {
    cout<<"MGCPGen (c) 2026 MaxGCoding.com"<<endl;
    cout<<name<<" <grammar file>"<<endl;
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        showUsage(argv[0]);
        return 1;
    }
    Grammar G;
    G.readGrammarFile(argv[1]);
    ParserGenerator pg;
    ParseTable table = pg.generate(G, G.startSym);
    pg.tableGen.persist(argv[2], G);
    return 0;
}