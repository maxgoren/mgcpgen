#ifndef build_table_hpp
#define build_table_hpp
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include "cfg.hpp"
using namespace std;

typedef map<Symbol, map<Symbol, Production>> ParseTable;


class TableGenerator {
    private:
        ParseTable table;
        set<Symbol> firstFromString(const vector<Symbol>& rhs, Grammar& G);
    public:
        TableGenerator() {

        }
        ParseTable makeParseTable(Grammar& G);
        void persist(string filename, Grammar& G);
};

set<Symbol> TableGenerator::firstFromString(const vector<Symbol>& rhs, Grammar& G) {
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
        if (find(G.terminals.begin(), G.terminals.end(), X) != G.terminals.end()) {
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


ParseTable TableGenerator::makeParseTable(Grammar& G) {
    table = ParseTable();
    for (auto A : G.nonterminals) {

        for (auto& prod : G.productions[A]) {

            auto firstAlpha = firstFromString(prod.rhs, G);

            // FIRST(α) - {ε}
            for (auto t : firstAlpha) {
                if (t != EPS) {
                    if (table[A].find(t) != table[A].end()) {
                        cout<<"Warning: Multiple Productions for M["<<A<<"]["<<t<<"]"<<endl;
                    }
                    cout << "M[" << A << "," << t << "] "
                         << prod.toString() << endl;
                    table[A][t] = prod;
                } 
            }

            // If ε ∈ FIRST(α), add FOLLOW(A)
            if (firstAlpha.find(EPS) != firstAlpha.end()) {
                for (auto b : G.follow[A]) {
                    if (table[A].find(b) != table[A].end()) {
                        cout<<"Warning: Multiple Productions for M["<<A<<"]["<<b<<"]"<<endl;
                    }
                    cout << "M[" << A << "," << b << "] "
                         << prod.toString() << endl;
                    table[A][b] = prod;

                }
            }
        }
    }
    return table;
}

void TableGenerator::persist(string filename, Grammar& G) {
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