#ifndef calc_firsts_hpp
#define calc_firsts_hpp
#include "cfg.hpp"

class ComputeFirstSets {
    private:
        bool debug_noise;
        void initFirsts(Grammar& G);
        bool updateNonTerminal(Grammar& G, Symbol X, Symbol f);
        bool firstClosure(Grammar& G, Symbol X, SymbolString& production);
        bool propagateFirsts(Grammar& G);
    public:
        ComputeFirstSets();
        void compute(Grammar& G);
        void printFirsts(Grammar& G);
};

#endif