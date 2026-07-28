#include <iostream>
#include "lr_gen/lr_gen.hpp"
using namespace std;

void showUsage(string name) {
    cout<<"MGCPGen (c) 2026 MaxGCoding.com"<<endl;
    cout<<name<<" <filename>"<<endl;
}


void generate_lr_parser(Grammar& G) {
    LRGenerator lrgen;
    auto[actTab, goTab] = lrgen.generate(G, "mgcpgen_out.hpp", "sp");
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        showUsage(argv[0]);
        return 1;
    }
    Grammar G;
    G.readGrammarFile(argv[1]);
    generate_lr_parser(G);
    return 0;
}