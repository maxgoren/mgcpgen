#ifndef build_table_hpp
#define build_table_hpp
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include "../../src/cfg.hpp"
using namespace std;

typedef map<Symbol, map<Symbol, Production>> ParseTable;

class LLTableGenerator {
    private:
        ParseTable table;
        set<Symbol> firstFromString(const vector<Symbol>& rhs, Grammar& G);
        void fillFromFirsts(Grammar& G, ParseTable& table, Symbol sym, Production& prod, const set<Symbol>& firstAlpha);
        void fillFromFollow(Grammar& G, ParseTable& table, Symbol sym, Production& prod, const set<Symbol>& firstAlpha);
    public:
        LLTableGenerator() {

        }
        ParseTable makeParseTable(Grammar& G);
        void persist(string filename, Grammar& G);
};

set<Symbol> LLTableGenerator::firstFromString(const vector<Symbol>& rhs, Grammar& G) {
    set<Symbol> result;
    if (rhs.empty()) {
        return {EPS};
    }
    bool allNullable = true;
    bool realSym = false;
    for (auto X : rhs) {
        if (X == ACTSYM)
            continue;
        realSym = true;
        if (G.terminals.find(X) != G.terminals.end()) {
            result.insert(X);
            return result;
        }

        for (auto t : G.firsts[X]) {
            if (t != EPS)
                result.insert(t);
        }

        if (G.firsts[X].find(EPS) == G.firsts[X].end()) {
            allNullable = false;
            break;
        }
    }

    if (!realSym || allNullable)
        result.insert(EPS);

    return result;
}

void LLTableGenerator::fillFromFirsts(Grammar& G, ParseTable& table, Symbol sym, Production& prod, const set<Symbol>& firstAlpha) {
    for (auto t : firstAlpha) {
        if (t != EPS) {
            if (table[sym].find(t) != table[sym].end()) {
                cout<<"Warning: Multiple Productions for M["<<sym<<"]["<<t<<"]"<<endl;
            }
            cout << "M[" << sym << "," << t << "] "<< prod.toString() << endl;
            table[sym][t] = prod;
        } 
    }
}

void LLTableGenerator::fillFromFollow(Grammar& G, ParseTable& table, Symbol sym, Production& prod, const set<Symbol>& firstAlpha) {
    for (auto b : G.follow[sym]) {
        if (table[sym].find(b) != table[sym].end()) {
            cout<<"Warning: Multiple Productions for M["<<sym<<"]["<<b<<"]"<<endl;
        }
        cout<<"M["<<sym<<","<<b<<"] "<< prod.toString() << endl;
        table[sym][b] = prod;
    }
}

ParseTable LLTableGenerator::makeParseTable(Grammar& G) {
    table = ParseTable();
    for (auto sym : G.nonterminals) {
        for (auto& prod : G.productions[sym]) {
            auto firstAlpha = firstFromString(prod.rhs, G);
            // FIRST(α) - {ε}
            fillFromFirsts(G, table, sym, prod, firstAlpha);
            // If ε ∈ FIRST(α), add FOLLOW(sym)
            if (firstAlpha.find(EPS) != firstAlpha.end()) {
                fillFromFollow(G, table, sym, prod, firstAlpha);
            }
        }
    }
    return table;
}

void LLTableGenerator::persist(string filename, Grammar& G) {
    std::ofstream ot(filename, ios::out);
    if (ot.good()) {
        ot<<"#include <vector>\n";
        ot<<"#include <map>\n";
        ot<<"#include <set>\n";
        ot<<"#include \"cfg.hpp\"\n";
        ot<<"using namespace std; \n";
        ot<<"set<Symbol> terminalSymbols = {";
        auto it = G.terminals.begin();
        while (it != G.terminals.end()) {
            ot<<"\""<<*it<<"\"";
            it++;
            if (it != G.terminals.end())
                ot<<", ";
        }
        ot<<"};"<<endl;
        ot<<"set<Symbol> nonTerminalSymbols = {";
        it = G.nonterminals.begin();
        while (it != G.nonterminals.end()) {
            ot<<"\""<<*it<<"\"";
            it++;
            if (it != G.nonterminals.end())
                ot<<", ";
        }
        ot<<"};"<<endl;
        ot<<"typedef map<Symbol, map<Symbol, Production>> ParseTable;\n";
        ot<<"ParseTable parseTable;\n";
        ot<<"void initParseTable() {\n";
        for (auto t : table) {
            for (auto e : t.second) {
                ot<<"\t parseTable[\""<<t.first<<"\"][\""<<e.first<<"\"] = Production("<<e.second.pid<<",\""<<t.first<<"\", ";
                ot<<"SymbolString({";
                for (int i = 0; i < e.second.rhs.size(); i++) {
                    ot<<"\""<<e.second.rhs[i]<<"\"";
                    if (i+1 < e.second.rhs.size())
                        ot<<",";
                }
                ot<<"}));"<<endl;
            }           
        }
        ot<<"}\n";
    }
    ot.close();
}

#endif