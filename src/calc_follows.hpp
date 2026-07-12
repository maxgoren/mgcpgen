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
        ComputeFollowSets();
        void compute(Grammar& G, Symbol start);
        void printFollows(Grammar& G);
};



#endif