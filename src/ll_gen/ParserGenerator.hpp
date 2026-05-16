#ifndef parser_generator_hpp
#define parser_generator_hpp
#include "../../src/cfg.hpp"
#include "../../src/calc_firsts.hpp"
#include "../../src/calc_follows.hpp"
#include "build_table.hpp"

class ParserGenerator {
    private:
        ComputeFirstSets    firsts;
        ComputeFollowSets   follows;
    public:
        LLTableGenerator      tableGen;
    public:
        ParserGenerator() {

        }
        ParseTable generate(Grammar& G, Symbol start) {
            firsts.compute(G);
            firsts.printFirsts(G);
            follows.compute(G, start);
            follows.printFollows(G);
            return tableGen.makeParseTable(G);
        }
};


#endif