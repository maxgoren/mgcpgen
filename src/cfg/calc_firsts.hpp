#ifndef calc_firsts_hpp
#define calc_firsts_hpp
#include "cfg.hpp"

class FirstSetCalculator {
    private:
        bool debug_noise;
        void initFirstsAndNullable(Grammar& G);
        bool updateNonTerminal(Grammar& G, Symbol X, Symbol f);
        bool firstClosure(Grammar& G, Symbol X, SymbolString& production);
    public:
        FirstSetCalculator();
        void compute(Grammar& G);
        void printFirsts(Grammar& G);
};

#endif