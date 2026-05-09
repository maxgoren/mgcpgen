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

class ComputeFollowSets {
    private:
        bool firstFromRHS(Grammar& G, Symbol A, Symbol X, SymbolString alt, int index);
        bool followClosure(Grammar& G, Symbol A);
    public:
        ComputeFollowSets() { }
        void compute(Grammar& G, Symbol start);
        void printFollows(Grammar& G);
};

bool ComputeFollowSets::firstFromRHS(Grammar& G, Symbol currNonTerm, Symbol currLHS, SymbolString alt, int index) {
    bool betaEps = true;
    bool didchange = false;
    bool realSym = false;
    // X -> aAb
    for (Symbol Y : alt.subString(index+1)) {
        if (Y == ACTSYM)
            continue;
        realSym = true;
        for (Symbol s : G.firsts[Y]) {
            if (s != EPS) {
                if (G.follow[currNonTerm].insert(s).second)
                    didchange = true;
            }
        }
        if (G.firsts[Y].find(EPS) == G.firsts[Y].end()) {
            betaEps = false;
            break;
        }
    }

    //X -> aA
    if (!realSym || betaEps) {
        int pre = G.follow[currNonTerm].size();
        G.follow[currNonTerm].insert(G.follow[currLHS].begin(), G.follow[currLHS].end());
        if (pre != G.follow[currNonTerm].size())
            didchange = true;
    }
    return didchange;
}

bool ComputeFollowSets::followClosure(Grammar& G, Symbol nonTerm) {
    bool didchange = false;
    for (auto prod : G.productions) {
        Symbol LHS = prod.first;
        ProductionSet RHS = prod.second;
        for (Production alt : RHS) {
            for (int i = 0; i < alt.rhs.size(); i++) {
                if (alt.rhs[i] == ACTSYM)
                    continue;
                if (alt.rhs[i] == nonTerm) {
                    if (firstFromRHS(G, nonTerm, LHS, alt.rhs, i))
                        didchange = true;
                }
            }
        }
    }
    return didchange;
}

void ComputeFollowSets::compute(Grammar& G, Symbol start) {
    for (Symbol t : G.terminals)     { G.follow[t] = set<Symbol>(); }
    for (Symbol nt : G.nonterminals) { G.follow[nt] = set<Symbol>(); }
    G.follow[start].insert(GOAL);
    bool didchange = true;
    while (didchange) {
        didchange = false;
        for (auto nonTerm : G.nonterminals) {
            if (followClosure(G, nonTerm))
                didchange = true;                
        }   
    }
}

void ComputeFollowSets::printFollows(Grammar& G) {
    for (auto f : G.follow) {
        if (G.isNonTerminal(f.first)) {
            cout<<"Follows("<<f.first<<"): { ";
            for (auto q : f.second) {
                cout<<q<<" ";
            }
            cout<<"}"<<endl;
        }
    }
}

#endif