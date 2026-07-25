#include "calc_follows.hpp"

FollowSetCalculator::FollowSetCalculator() { }

bool FollowSetCalculator::firstFromRHS(Grammar& G, Symbol currNonTerm, Symbol currLHS, SymbolString alt, int index) {
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

/*
    For Each Production
        examing each production rule symbol by symbol
        If we encounter the Symbol <nonTerm>:
            call firstFromRhs using current position as index
*/
bool FollowSetCalculator::followClosure(Grammar& G, Symbol nonTerm) {
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


/*
    Loop over the set of non-terminal's, calling the closure operation
    on each until we go through the list without making any changes.
    If changes do occur, repeat.
*/
void FollowSetCalculator::compute(Grammar& G, Symbol start) {
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

void FollowSetCalculator::printFollows(Grammar& G) {
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