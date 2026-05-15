#include "slr_gen.hpp"
void showUsage(string name) {
    cout<<"MGCPGen, SLR Generator"<<endl;
    cout<<name<<" <filename> "<<endl;
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        showUsage(argv[0]);
        return 1;
    }
    Grammar G;
    G.readGrammarFile(argv[1]);
    SLRGenerator slrgen;
    auto[actTab, goTab] = slrgen.generate(G, "sp");
    return 0;
}