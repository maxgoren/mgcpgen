#ifndef derives_lambda_hpp
#define derives_lambda_hpp
#include "cfg.hpp"

class DerivesLambda {
    private:
        bool debug_noise;
    public:
        DerivesLambda();
        void markNonTerminals(Grammar& G);
};


#endif