#ifndef production_h
#define production_h
#include <vector>
#include <iostream>
using namespace std;

using Symbol = string;

struct SymbolString : vector<Symbol> {
    SymbolString(vector<Symbol> ss);
    SymbolString();
    SymbolString(const SymbolString& ss);
    SymbolString subString(int startIndex);
    SymbolString& operator=(const SymbolString& ss);
    string toString();
    bool operator==(const SymbolString& ss);
    bool operator!=(const SymbolString& ss);
};

struct Production {
    int pid; //for executing Actions
    Symbol lhs;
    SymbolString rhs;
    string action;
    Symbol precOverride;
    Production(int id, Symbol l, SymbolString r, string a) ;
    Production(int id, Symbol l, SymbolString r) ;
    Production();
    string toString();
    bool operator==(const Production& op) const;
    bool operator!=(const Production& op) const;
    bool operator<(const Production& op) const;
};


#endif