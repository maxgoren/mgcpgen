#include <iostream>
#include "lr_gen/slr_gen.hpp"
using namespace std;

void showUsage(string name) {
    cout<<"MGCPGen (c) 2026 MaxGCoding.com"<<endl;
    cout<<name<<" <filename>"<<endl;
}


void generate_slr_parser(Grammar& G) {
    SLRGenerator slrgen;
    auto[actTab, goTab] = slrgen.generate(G, "mgc_slr_gen.out.hpp", "sp");
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        showUsage(argv[0]);
        return 1;
    }
    Grammar G;
    G.readGrammarFile(argv[1]);
    generate_slr_parser(G);
    return 0;
}