#include <iostream>
#include "lr_gen/lr_gen.hpp"
using namespace std;

void showUsage(string name) {
    cout<<"MGCPGen (c) 2026 MaxGCoding.com"<<endl;
    cout<<name<<" <filename> [<output filename>]"<<endl;
}

const static string DEF_OUT_FNAME = "mgcpgen_out.hpp";

void generate_lr_parser(string in_file, string out_file) {
    LRGenerator lrgen;
    Grammar G;
    cout<<"[*] Reading Grammar File: "<<in_file<<endl;
    G.readGrammarFile(in_file);
    auto[actTab, goTab] = lrgen.generate(G, out_file);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        showUsage(argv[0]);
        return 1;
    }
    switch (argc) {
        case 3:
            generate_lr_parser(argv[1], argv[2]);
            break;
        case 2: 
        default:
            generate_lr_parser(argv[1], DEF_OUT_FNAME);
            break;
    }
    return 0;
}