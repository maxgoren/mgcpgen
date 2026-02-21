#ifndef build_table_hpp
#define build_table_hpp
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include "cfg.hpp"
using namespace std;

set<Symbol> firstOfSequence(const vector<Symbol>& rhs, Grammar& G) {
    set<Symbol> result;
    if (rhs.empty()) {
        return {EPS};
    }
    bool allNullable = true;
    for (auto X : rhs) {

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

    if (allNullable)
        result.insert(EPS);

    return result;
}

typedef map<Symbol, map<Symbol, Production>> ParseTable;

ParseTable makeParseTable(Grammar& G) {
    ParseTable table;
    for (auto A : G.nonterminals) {

        for (auto& prod : G.productions[A]) {

            auto firstAlpha = firstOfSequence(prod.rhs, G);

            // FIRST(α) - {ε}
            for (auto t : firstAlpha) {
                if (t != EPS) {
                    cout << "M[" << A << "," << t << "] "
                         << prod.toString() << endl;
                    table[A][t] = prod;
                } 
            }

            // If ε ∈ FIRST(α), add FOLLOW(A)
            if (firstAlpha.find(EPS) != firstAlpha.end()) {
                for (auto b : G.follow[A]) {
                    cout << "M[" << A << "," << b << "] "
                         << prod.toString() << endl;
                    table[A][b] = prod;

                }
            }
        }
    }
    return table;
}

#endif