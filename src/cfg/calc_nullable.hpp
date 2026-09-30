#ifndef calc_nullable_hpp
#define calc_nullable_hpp
#include "cfg.hpp"

class NullableCalculator {
    private:
        bool debug_noise;
    public:
        NullableCalculator();
        void compute(Grammar& G);
};


#endif