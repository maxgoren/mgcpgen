#ifndef cfg_hpp
#define cfg_hpp
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <map>
#include <set>
#include "production.hpp"
using namespace std;

using Symbol = int;

const string ACTSYM = "@";
const string EPS = "#";
const string GOAL = "TK_EOI";
vector<string> split(string input, char delim);

struct ProductionSet : vector<Production> {
    ProductionSet(vector<Production> rhs);
    ProductionSet();
};

struct OpPrec {
    Symbol sym;
    int prec_level;
    string assoc;
    OpPrec(Symbol s = -1, int p = 0, string a = 0) : sym(s), prec_level(p), assoc(a) { }
};


class SymbolMap {
    private:
        vector<string> idToName;
        unordered_map<string, int> nameToId;
    public:
        SymbolMap() {

        }
        int size() {
            return nameToId.size();
        }
        int get(string name) {
            if (name.empty())
                return -1;
            auto it = nameToId.find(name);
            if (it != nameToId.end()) return it->second;
            int next = nameToId.size();
            nameToId.insert(make_pair(name, next));
            idToName.push_back(name);
            return next;
        }
        string name(Symbol sym) {
            if (sym < 0 || sym > idToName.size())
                return "";
            return idToName[sym];
        }
};

struct Grammar {
    SymbolMap symMap;
    Symbol startSym;
    string returnType;
    set<Symbol> terminals;
    set<Symbol> nonterminals;
    map<Symbol, ProductionSet> productions;
    map<Symbol, bool> nullable;
    map<Symbol, set<Symbol>> firsts;
    map<Symbol, set<Symbol>> follow;
    map<int, Production> prodById;
    map<Symbol, OpPrec> precedenceMap;
    map<string, string> actionMap;
    Grammar() ;
    bool isNonTerminal(Symbol s);
    bool isTerminal(Symbol s);
    bool isNullable(Symbol nt);
    bool readGrammarFile(string filename, bool and_lexer);
    void parseProduction(vector<string>& parts, int rulenum);
    void populateTerminalList(string filename, bool generate_lexer);
};

vector<string> split(string input, char delim);

#endif