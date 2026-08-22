#ifndef calc_nullable_hpp
#define calc_nullable_hpp
#include "cfg.hpp"

class CalculateNullable {
    private:
        bool debug_noise;
    public:
        CalculateNullable();
        void compute(Grammar& G);
};


#endif