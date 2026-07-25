#include "calc_firsts.hpp"

FirstSetCalculator::FirstSetCalculator() {
    debug_noise = false;
}

void  FirstSetCalculator::initFirsts(Grammar& G) {
    //For all terminal symbols, first(t) -> {t}
    for (Symbol t : G.terminals) {
        G.firsts[t] = {t};
    }
    //create a first set for each non terminal, 
    //if the non terminal can derive epsilon, add epsilon to its set.
    for (Symbol nt : G.nonterminals) {
        G.firsts[nt] = set<Symbol>();
        if (G.isNullable(nt)) {
            if (debug_noise)
                cout<<nt<<" is nullable..."<<endl;
            G.firsts[nt].insert("#");
        }
    }
}

bool FirstSetCalculator::updateNonTerminal(Grammar& G, Symbol X, Symbol f) {
    bool didchange = false;
    for (auto k : G.firsts[f]) {
        if (G.firsts[X].find(k) == G.firsts[X].end() && k != EPS) {
            G.firsts[X].insert(k);
            didchange = true;  // Mark that a change occurred
        }
    }
    return didchange;
}

bool FirstSetCalculator::firstClosure(Grammar& G, Symbol X, SymbolString& production) {
    bool didchange = false;
     // Check if the first symbol of this production is epsilon
     if (production.empty()) {
        G.firsts[X].insert(EPS);
    } else {
        Symbol firstSymbol = production.front() == ACTSYM ? *(production.begin()+1):production.front();
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

bool FirstSetCalculator::propagateFirsts(Grammar& G) {
    bool didchange = false;
    for (auto prod : G.productions) {
        Symbol X = prod.first;  // Left-hand side non-terminal
        ProductionSet RHS = prod.second;
        // Process each alternative production for X
        for (Production production : RHS) {
            if (firstClosure(G, X, production.rhs))
                didchange = true;
        }
    }   
    return didchange;
}

void FirstSetCalculator::compute(Grammar& G) {
    // Initialize FIRST sets for terminals and non-terminals
    initFirsts(G);
    
    // Iterative algorithm to compute FIRST sets until no changes occur
    bool didchange = true;
    while (didchange) {
        didchange = propagateFirsts(G);
    }
} 

void FirstSetCalculator::printFirsts(Grammar& G) {
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