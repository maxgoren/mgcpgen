#include "calc_nullable.hpp"

NullableCalculator::NullableCalculator() {
    debug_noise = true;
}

void NullableCalculator::compute(Grammar& G) {
    for (auto nt : G.nonterminals) {
        G.nullable[nt] = false;
    }
    bool changed = false;
    do {
        changed = false;
        for (auto & [id, prod] : G.prodById) {
            if (!G.nullable[prod.lhs]) {
                if (prod.rhs.empty()) {
                    changed = true;
                    G.nullable[prod.lhs] = true;
                    if (debug_noise) {
                        cout<<"\t \t+ "<<prod.lhs<<" derives lambda"<<endl;
                    }
                    continue;
                }
                bool rhs_derives = G.nullable[prod.rhs[0]];
                for (int j = 1; j < prod.rhs.size(); j++) {
                    rhs_derives = rhs_derives && G.nullable[prod.rhs[j]];
                }
                if (rhs_derives) {
                    changed = true;
                    G.nullable[prod.lhs] = true;
                    if (debug_noise) {
                        cout<<"\t \t+ "<<prod.lhs<<" derives lambda"<<endl;
                    }
                }
            }
        }
    } while (changed);
}