#ifndef parser_generator_hpp
#define parser_generator_hpp
#include "cfg.hpp"
#include "calc_firsts.hpp"
#include "calc_follows.hpp"
#include "build_table.hpp"

class ParserGenerator {
    private:
        ComputeFirstSets    firsts;
        ComputeFollowSets   follows;
    public:
        TableGenerator      tableGen;
    public:
        ParserGenerator() {

        }
        ParseTable generate(Grammar& G, Symbol start) {
            firsts.compute(G);
            follows.compute(G, start);
            return tableGen.makeParseTable(G);
        }
};


#endif