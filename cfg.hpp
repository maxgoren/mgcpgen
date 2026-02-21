#ifndef cfg_hpp
#define cfg_hpp
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
using namespace std;

using Symbol = string;

struct SymbolString : vector<Symbol> {
    SymbolString(vector<Symbol> ss) {
        for (auto m : ss) {
            this->push_back(m);
        }
    }
    SymbolString() {

    }
    SymbolString(const SymbolString& ss) {
        for (auto m : ss) {
            this->push_back(m);
        }
    }
    SymbolString subString(int startIndex) {
        SymbolString result;
        for (int i = startIndex; i < this->size(); i++)
            result.push_back((*this)[i]);
        return result;
    }
    SymbolString& operator=(const SymbolString& ss) {
        if (this != &ss) {
            for (auto m : ss) {
                this->push_back(m);
            }
        }
        return *this;
    }
    string toString() {
        string str = "";
        for (Symbol s : *this) {
            str += s + " ";
        }
        return str;
    }
    bool operator==(const SymbolString& ss) {
        auto oit = ss.begin();
        auto it = this->begin();
        while (oit != ss.end() && it != this->end()) {
            if (*oit != *it)
                return false;
        }
        return (oit == ss.end() && it == this->end());
    }
    bool operator!=(const SymbolString& ss) {
        return !(*this == ss);
    }
};

struct Production {
    int pid; //for executing Actions
    Symbol lhs;
    SymbolString rhs;
    Production(int id, Symbol l, SymbolString r) : pid(id), lhs(l), rhs(r) { }
    Production() { }
    string toString() {
        return lhs + " ::= " + rhs.toString(); 
    }
    bool operator==(const Production& op) {
        return lhs == op.lhs && rhs == op.rhs;
    }
    bool operator!=(const Production& op) {
        return !(*this==op);
    }
    bool operator<(const Production& op) const {
        return this->lhs < op.lhs;
    }
};

struct ProductionSet : vector<Production> {
    ProductionSet(vector<Production> rhs) {
        for (Production ss : rhs) {
            push_back(ss);
        }
    }
    ProductionSet() {

    }
};

struct ParseState {
    Symbol nonTerm;
    Symbol term;
    ParseState(Symbol NT, Symbol T) {
        nonTerm = NT;
        term = T;
    }
    bool operator<(const ParseState& ps) const {
        return nonTerm < ps.nonTerm;
    }
};

const string EPS = "#";
const string GOAL = "TK_EOI";



struct Grammar {
    vector<Symbol> terminals;
    vector<Symbol> nonterminals;
    map<Symbol, ProductionSet> productions;
    map<Symbol, set<Symbol>> firsts;
    map<Symbol, set<Symbol>> follow;
    map<ParseState, SymbolString> predict;
    bool isNonTerminal(Symbol s) {
        return find(nonterminals.begin(), nonterminals.end(), s) != nonterminals.end();
    }
    bool isTerminal(Symbol s) {
        return find(terminals.begin(), terminals.end(), s) != terminals.end();
    }
    bool isNullable(Symbol nt) {
        for (auto prod : productions[nt]) {
            if (prod.rhs[0] == EPS)
                return true;
        }
        return false;
    }
};

#endif