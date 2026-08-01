#include <iostream>
#include "lr_gen/lr_gen.hpp"
using namespace std;

void showUsage(string name) {
    cout<<"MGCPGen (c) 2026 MaxGCoding.com"<<endl;
    cout<<name<<" <parser type> <filename> [<output filename>]"<<endl;
    cout<<"        -c for canonical LR(1) parser"<<endl;
    cout<<"        -l for LALR(1) parser"<<endl;
}

const static string DEF_OUT_FNAME = "mgcpgen_out.hpp";

void generate_lr_parser(char parserType, string in_file, string out_file) {
    LRGenerator lrgen(false, parserType == 'c' ? CLR:LALR);
    Grammar G;
    cout<<"[*] Reading Grammar File: "<<in_file<<endl;
    if (G.readGrammarFile(in_file)) {
        auto[actTab, goTab] = lrgen.generate(G, out_file);
    } else {
        cout<<"[*] Failed."<<endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        showUsage(argv[0]);
        return 1;
    }
    switch (argc) {
        case 4:
            generate_lr_parser(argv[1][1], argv[2], argv[3]);
            break;
        case 3: 
        default:
            generate_lr_parser(argv[1][1], argv[2], DEF_OUT_FNAME);
            break;
    }
    return 0;
}