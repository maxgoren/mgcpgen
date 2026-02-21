#ifndef calc_follows_hpp
#define calc_follows_hpp
#include "cfg.hpp"
#include "calc_firsts.hpp"
/**
 * Rules for Follow Sets
 *
 * - First put $ (the end of input marker) in Follow(S) (S is the start symbol)
 * - If there is a production A → aBb, (where a can be a whole string)
 *   then everything in FIRST(b) except for ε is placed in FOLLOW(B).
 * - If there is a production A → aB, then everything in
 *   FOLLOW(A) is in FOLLOW(B)
 * - If there is a production A → aBb, where FIRST(b) contains ε,
 *   then everything in FOLLOW(A) is in FOLLOW(B)
 */

 bool symbolFoundOnRight(Grammar& G, Symbol A, Symbol X, SymbolString alt, int index) {
    bool betaEps = true;
    bool didchange = false;
    // X -> aAb
    for (Symbol Y : alt.subString(index+1)) {
        for (Symbol s : G.firsts[Y]) {
            if (s != EPS) {
                if (G.follow[A].insert(s).second)
                    didchange = true;
            }
        }
        if (G.firsts[Y].find(EPS) == G.firsts[Y].end()) {
            betaEps = false;
            break;
        }
    }

    //X -> aA
    if (index+1 == alt.size() || betaEps) {
        int pre = G.follow[A].size();
        G.follow[A].insert(G.follow[X].begin(), G.follow[X].end());
        if (pre != G.follow[A].size())
            didchange = true;
    }
    return didchange;
 }

bool calcFollow(Grammar& G, Symbol A) {
    bool didchange = false;
    for (auto prod : G.productions) {
        Symbol X = prod.first;
        ProductionSet RHS = prod.second;
        for (Production alt : RHS) {
            for (int i = 0; i < alt.rhs.size(); i++) {
                if (alt.rhs[i] == A) {
                    if (symbolFoundOnRight(G, A, X, alt.rhs, i))
                        didchange = true;
                }
            }
        }
    }
    return didchange;
}

void initFollows(Grammar& G) {
    for (Symbol t : G.terminals) {
        G.follow[t] = set<Symbol>();
    }
    for (Symbol nt : G.nonterminals) {
        G.follow[nt] = set<Symbol>();
    }
}

void calcFollowSets(Grammar& G, Symbol start) {
    initFollows(G);
    G.follow[start].insert(GOAL);
    bool didchange = true;
    while (didchange) {
        didchange = false;
        for (auto A : G.nonterminals) {
            if (calcFollow(G, A))
                didchange = true;                
        }   
    }
}

void printFollows(Grammar& G) {
    for (auto f : G.follow) {
        if (G.isNonTerminal(f.first)) {
            cout<<"Follows("<<f.first<<": { ";
            for (auto q : f.second) {
                cout<<q<<" ";
            }
            cout<<"}"<<endl;
        }
    }
}

#endif