#ifndef calc_firsts_hpp
#define calc_firsts_hpp
#include "cfg.hpp"

void initFirsts(Grammar& G) {
    //For all terminal symbols, first(t) -> {t}
    for (Symbol t : G.terminals) {
        G.firsts[t] = {t};
    }
    //create a first set for each non terminal, 
    //if the non terminal can derive epsilon, add epsilon to its set.
    for (Symbol nt : G.nonterminals) {
        G.firsts[nt] = set<Symbol>();
        if (G.isNullable(nt)) {
            cout<<nt<<" is nullable..."<<endl;
            G.firsts[nt].insert("#");
        }
    }
}

bool updateNonTerminal(Grammar& G, Symbol X, Symbol f) {
    bool didchange = false;
    for (auto k : G.firsts[f]) {
        if (G.firsts[X].find(k) == G.firsts[X].end() && k != EPS) {
            G.firsts[X].insert(k);
            didchange = true;  // Mark that a change occurred
        }
    }
    return didchange;
}

bool propagate(Grammar& G, Symbol X, SymbolString& production) {
    bool didchange = false;
     // Check if the first symbol of this production is epsilon
    Symbol firstSymbol = production.front();
     if (firstSymbol == EPS) {
        G.firsts[X].insert(EPS);
    } else {
        // If f is a non-terminal
        if (G.firsts.find(firstSymbol) != G.firsts.end()) {
            // Add all symbols from FIRST(firstSymbol) to FIRST(X), except epsilon if f is nullable (FIRST(f) - 'E')
            didchange = updateNonTerminal(G, X, firstSymbol);
        } else if (G.firsts[X].find(firstSymbol) == G.firsts[X].end()) {
            G.firsts[X].insert(firstSymbol);
            didchange = true;  // Mark that a change occurred
        }
    }
    return didchange;
}

bool propagateFirsts(Grammar& G) {
    bool didchange = false;
    for (auto prod : G.productions) {
        Symbol X = prod.first;  // Left-hand side non-terminal
        ProductionSet RHS = prod.second;
        // Process each alternative production for X
        for (Production production : RHS) {
            if (propagate(G, X, production.rhs))
                didchange = true;
        }
    }   
    return didchange;
}

void calcFirstSets(Grammar& G) {
    // Initialize FIRST sets for terminals and non-terminals
    initFirsts(G);
    
    // Iterative algorithm to compute FIRST sets until no changes occur
    bool didchange = true;
    int iter = 0;
    while (didchange) {
        // Reset change flag for this iteration
        didchange = propagateFirsts(G);
    }
} 

void printFirsts(Grammar& G) {
    for (auto f : G.firsts) {
        if (G.isNonTerminal(f.first)) {
            cout<<"FIRST("<<f.first<<")"<<": { ";
            for (auto q : f.second) {
                cout<<q<<" ";
            }
            cout<<"}"<<endl;
        }
    }
}

#endif