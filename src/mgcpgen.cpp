#include <iostream>
#include "slr_gen/slr_gen.hpp"
#include "ll_gen/ParserGenerator.hpp"
using namespace std;
void showUsage(string name) {
    cout<<"MGCPGen (c) 2026 MaxGCoding.com"<<endl;
    cout<<name<<" -ll <filename> for LL(1) grammars"<<endl;
    cout<<name<<" -slr <filename> for SLR(1) grammars"<<endl;
}


void generate_slr_parser(Grammar& G) {
    SLRGenerator slrgen;
    auto[actTab, goTab] = slrgen.generate(G, "sp");
}

void generate_ll_parser(Grammar& G) {
    ParserGenerator pg;
    pg.generate(G, "prog");
    pg.tableGen.persist("mgc_ll.out.hpp", G);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        showUsage(argv[0]);
        return 1;
    }
    Grammar G;
    G.readGrammarFile(argv[2]);
    switch (argv[1][1]) {
        case 's': generate_slr_parser(G); break;
        case 'l': generate_ll_parser(G);  break;
    }
    return 0;
}