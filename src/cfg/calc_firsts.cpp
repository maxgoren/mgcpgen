#include "calc_firsts.hpp"

FirstSetCalculator::FirstSetCalculator() {
    debug_noise = true;
}

void  FirstSetCalculator::initFirsts(Grammar& G) {
    //For all terminal symbols, first(t) -> {t}
    for (Symbol t : G.terminals) {
        G.firsts[t] = {t};
        G.derivesLambda[t] = false;
    }
    //create a first set for each non terminal, 
    //if the non terminal can derive epsilon, add epsilon to its set.
    for (Symbol nt : G.nonterminals) {
        G.firsts[nt] = set<Symbol>();
        G.derivesLambda[nt] = false;
    }
    bool changed = false;
    do {
        changed = false;
        for (auto & [id, prod] : G.prodById) {
            if (!G.derivesLambda[prod.lhs]) {
                if (prod.rhs.empty()) {
                    changed = true;
                    G.derivesLambda[prod.lhs] = true;
                    G.firsts[prod.lhs].insert("#");
                    if (debug_noise)
                        cout<<prod.lhs<<" is nullable..."<<endl;
                    continue;
                }
                bool rhs_derives = G.derivesLambda[prod.rhs[0]];
                for (int j = 1; j < prod.rhs.size(); j++) {
                    rhs_derives = rhs_derives && G.derivesLambda[prod.rhs[j]];
                }
                if (rhs_derives) {
                    changed = true;
                    G.derivesLambda[prod.lhs] = true;
                    G.firsts[prod.lhs].insert("#");
                    if (debug_noise)
                        cout<<prod.lhs<<" is nullable..."<<endl;
                }
            }
        }
    } while (changed);
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
        Symbol firstSymbol = production.front();
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

void FirstSetCalculator::compute(Grammar& G) {
    // Initialize FIRST sets for terminals and non-terminals
    initFirsts(G);
    bool didchange = true;
    while (didchange) {
        didchange = false;
        for (auto [X, RHS] : G.productions) {
            // Process each alternative production for X
            for (Production production : RHS) {
                if (firstClosure(G, X, production.rhs)) {
                    didchange = true;
                }
            }
        }  
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