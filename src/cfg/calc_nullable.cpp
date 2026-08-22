#include "calc_nullable.hpp"

CalculateNullable::CalculateNullable() {
    debug_noise = true;
}

void CalculateNullable::compute(Grammar& G) {
    for (auto nt : G.nonterminals) {
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
                    if (debug_noise) {
                        cout<<"\t \t+ "<<prod.lhs<<" derives lambda"<<endl;
                    }
                    continue;
                }
                bool rhs_derives = G.derivesLambda[prod.rhs[0]];
                for (int j = 1; j < prod.rhs.size(); j++) {
                    rhs_derives = rhs_derives && G.derivesLambda[prod.rhs[j]];
                }
                if (rhs_derives) {
                    changed = true;
                    G.derivesLambda[prod.lhs] = true;
                    if (debug_noise) {
                        cout<<"\t \t+ "<<prod.lhs<<" derives lambda"<<endl;
                    }
                }
            }
        }
    } while (changed);
}