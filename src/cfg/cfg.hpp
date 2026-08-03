#ifndef cfg_hpp
#define cfg_hpp
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include "production.hpp"
using namespace std;

using Symbol = string;

const Symbol ACTSYM = "@";
const Symbol EPS = "#";
const Symbol GOAL = "TK_EOI";
vector<string> split(string input, char delim);

struct ProductionSet : vector<Production> {
    ProductionSet(vector<Production> rhs);
    ProductionSet();
};

struct OpPrec {
    Symbol sym;
    int prec_level;
    string assoc;
    OpPrec(Symbol s = "", int p = 0, string a = 0) : sym(s), prec_level(p), assoc(a) { }
};

struct Grammar {
    Symbol startSym;
    set<Symbol> terminals;
    set<Symbol> nonterminals;
    map<Symbol, ProductionSet> productions;
    map<Symbol, set<Symbol>> firsts;
    map<Symbol, set<Symbol>> follow;
    map<int, Production> prodById;
    map<Symbol, OpPrec> precedenceMap;
    map<string, string> actionMap;
    Grammar() ;
    bool isNonTerminal(Symbol s);
    bool isTerminal(Symbol s);
    bool isNullable(Symbol nt);
    bool readGrammarFile(string filename);
};

vector<string> split(string input, char delim);

#endif