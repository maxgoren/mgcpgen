#ifndef read_lexer_config_hpp
#define read_lexer_config_hpp
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

//this is all very much re-produced from mgclex, except now its used for populating
//set of terminal symbols for LR table gen

bool isWhitespace(char c) {
    return c == ' ' || c == '\t';
}

void skipWs(char* str, int* pos) {
    while (isWhitespace(str[*pos])) *pos++;
}

string extractPattern(char* str, int* pos) {
    string pattern;
    int i = *pos, j = 0;
    if (str[i] == '"') i++;
    while (str[i]) {
        if (str[i] == '"' && str[i-1] != '\\' && str[i+1] == ',')
            break;
        pattern.push_back(str[i++]);
    }
    if (str[i] == '"') i++;
    *pos = i;
    return pattern;
}

string extractSymbol(char* tmp, int i) {
    string tmp2;
    skipWs(tmp, &i);
    while (tmp[i] && tmp[i] != '}') { tmp2.push_back(tmp[i++]); }
    return tmp2;
}


vector<string> readConfig(string filename) {
    string buffer;
    vector<string> symbols;
    ifstream fd(filename);
    if (fd.is_open()) {
        printf("Loading Terminal Symbols from: %s\n", filename.data());
        int lineno = 0;
        while (fd.good()) {
            getline(fd, buffer);
            int pos = 0;
            ++lineno;
            while (buffer[pos] && buffer[pos] != '{') pos++;
            if (buffer[pos] == '{') {
                pos++;
            } else {
                printf("Error: improperly formatted specification file on line %d.\n", lineno);
                exit(EXIT_FAILURE);
            }
            string pat = extractPattern(buffer.data(), &pos);
            if (buffer[pos] == ',') {
                pos++;
            } else {
                printf("Error: improperly formatted specification file on line %d.\n", lineno);
                exit(EXIT_FAILURE);
            }                                                                                                                                                                                                                   
            string symbol = extractSymbol(buffer.data(), pos);
            printf("\t + Terminal Symbol: %s\n", symbol.data());
            symbols.push_back(symbol);
            printf("------------------------\n");
        }
        fd.close();
    } else {
        printf("Error: could not open specification file '%s' for reading.\n", filename.data());
        exit(EXIT_FAILURE);
    }
    return symbols;
}

#endif