#ifndef slr_gen_hpp
#define slr_gen_hpp
#include <iostream>
#include <functional>
#include "../../src/cfg/cfg.hpp"
#include "../../src/cfg/calc_firsts.hpp"
#include "../../src/cfg/calc_follows.hpp"
#include "lr_item.hpp"
#include "lr_state.hpp"
#include "directed_graph.hpp"
#include <stack>
#include <unordered_set>
using namespace std;


const int left_assoc = 10;
const int right_assoc = 20;

using GoToTable = map<int,map<Symbol,int>>;
using ActionTable = map<int,map<Symbol,string>>;

class SLRGenerator {
    private:
        unordered_set<Symbol> symbols;
        ComputeFirstSets    firsts;
        ComputeFollowSets   follows;
        DirectedGraph       cfsm;
        vector<LRState>     states;
        bool debug_noise;
        Symbol get_production_precedence_symbol(const Production& p, const Grammar& G);
        GoToTable make_goto_table(Grammar& G);
        ActionTable make_action_table(Grammar& G, Symbol ss);
        LRState closure(const Grammar& G, const LRState& state);
        LRState lr_goto(Grammar& G, const LRState& state, Symbol X);
        unordered_set<Symbol> firstFromSequence(Grammar& G, SymbolString seq); 
        void generate_CFSM(Grammar& G, Symbol ss);
        void printPrelude(ostream& ofile);
        void printProductions(ostream& os, Grammar& G, string name);
        template <class Iterable>
        void printTables(ostream& os, Iterable table, string tableName);
        pair<ActionTable, GoToTable> generate(Grammar& G, Symbol ss, ofstream& ofile) ;
    public:
        SLRGenerator(bool debug_noise = false);
        vector<LRState>& getStates();
        pair<ActionTable,GoToTable> generate(Grammar& G,  string outname, Symbol start);
        pair<ActionTable,GoToTable> generate(string filename, string outname,Symbol start);
};

#endif