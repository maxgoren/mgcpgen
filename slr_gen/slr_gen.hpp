#ifndef slr_gen_hpp
#define slr_gen_hpp
#include <iostream>
#include <functional>
#include "../src/cfg.hpp"
#include "../src/calc_firsts.hpp"
#include "../src/calc_follows.hpp"
#include "lr_item_set.hpp"
#include <stack>
#include "lexer.hpp"
using namespace std;


struct Transition {
    int dest;
    Symbol edgeLabel;
    Transition* next;
    Transition(int d, Symbol x, Transition* t) : dest(d), edgeLabel(x), next(t) { }
};

class DirectedGraph {
    private:
        map<int, Transition*> adjlist;
        int edgecount;
    public:
        DirectedGraph() {
            edgecount = 0;
        }
        int V() {
            return adjlist.size();
        }
        int E() {
            return edgecount;
        }
        void addEdge(int s, int t, Symbol X) {
            adjlist[s] = new Transition(t, X, adjlist[s]);
            edgecount++;
        }
        Transition* adj(int v) {
            return adjlist[v];
        }
        void print() {
            for (auto e : adjlist) {
                for (auto it = e.second; it != nullptr; it = it->next) {
                    cout<<e.first<<" -("<<it->edgeLabel<<")-> "<<it->dest<<endl;
                }
            }
        }
};

using GoToTable = map<int,map<Symbol,int>>;
using ActionTable = map<int,map<Symbol,string>>;

class SLRGenerator {
    private:
        unordered_set<Symbol> symbols;
        ComputeFirstSets    firsts;
        ComputeFollowSets   follows;
        DirectedGraph       cfsm;
        vector<LRState>     states;

        GoToTable make_goto_table(Grammar& G) {
            GoToTable tab;
            for (int s = 0; s < cfsm.V(); s++) {
                for (auto it = cfsm.adj(s); it != nullptr; it = it->next) {
                        tab[s][it->edgeLabel] = it->dest;
                }
            }
            return tab;
        } 
        ActionTable make_action_table(Grammar& G, Symbol ss) {
            ActionTable tab;
            for (int s = 0; s < cfsm.V(); s++) {
                for (auto it = cfsm.adj(s); it != nullptr; it = it->next) {
                    if (G.terminals.count(it->edgeLabel)) {
                        tab[s][it->edgeLabel] = "s"+to_string(it->dest);
                    }
                }
            }
            for (int s = 0; s < states.size(); s++) {
                for (const LRItem& item : states[s].items) {
                    if (!item.complete()) continue;
                    Production p = item.production;
                    if (p.lhs == ss) {
                        tab[s]["$"] = "accept";
                        continue;
                    }
                    for (Symbol a : G.follow.at(p.lhs)) {
                        tab[s][a] = "r"+to_string(p.pid);
                    }
                }
            }
            return tab;
        }
        void generate_CFSM(Grammar& G, Symbol ss) {
            for (Symbol s : G.terminals)
                symbols.insert(s);
            for (Symbol s : G.nonterminals)
                symbols.insert(s);
            LRState start;
            start.items.insert(LRItem(G.productions[ss][0], 0));
            LRState I0 = closure(G, start);
            queue<LRState> fq;
            unordered_set<string> seen;
            
            I0.state_num = 0;
            fq.push(I0);
            seen.insert(I0.key());
            states.push_back(I0);
            while (!fq.empty()) {
                cout<<"--------------------"<<endl;
                LRState curr = fq.front(); fq.pop();
                cout<<"Current state: "<<curr.state_num<<endl;
                unordered_set<Symbol> valid;
                for (auto item : curr.items) {
                    Symbol tmp = item.symbolAfterDot();
                    if (tmp != "<fin>")
                        valid.insert(tmp);
                }
                for (auto X : valid) {
                    LRState gt = lr_goto(G, curr, X);
                    if (gt.items.empty())
                        continue;
                    int target;
                    if (seen.find(gt.key()) == seen.end()) {
                        cout<<"\nGoto(I"<<curr.state_num<<","<<X<<"): "<<endl;
                        gt.state_num = states.size();
                        fq.push(gt);
                        states.push_back(gt);
                        seen.insert(gt.key());
                        target = gt.state_num;
                    } else {
                        for (auto s : states) {
                            if (gt.key() == s.key()) {
                                target = s.state_num;
                                cout<<"Found existing: "<<target<<endl;
                                break;
                            }
                        }
                    }
                    cfsm.addEdge(curr.state_num, target, X);                    
                }
            }
            for (auto m : states) {
                cout<<"I"<<m.state_num<<": \n";
                cout<<m.key();
            }
            cfsm.print();
        }
        template <class Iterable>
        void printTables(Iterable table) {
            for (auto e : table) {
                cout<<e.first<<": ";
                for (auto t : e.second) {
                    cout<<"("<<t.first<<", "<<t.second<<") ";
                }
                cout<<endl;
            }
        }
    public:
        SLRGenerator() {

        }
        vector<LRState>& getStates() {
            return states;
        }
        pair<ActionTable, GoToTable> generate(Grammar& G, Symbol ss) {
            firsts.compute(G);
            follows.compute(G, ss);
            generate_CFSM(G, ss);
            ActionTable actTable;
            GoToTable goTab;                                                                                                                                                                               
            goTab = make_goto_table(G);
            actTable = make_action_table(G, ss);
            printTables(goTab);
            printTables(actTable);
            firsts.printFirsts(G);
            follows.printFollows(G);
            return make_pair(actTable, goTab);
        }
        pair<ActionTable,GoToTable> generate(string filename, Symbol start) {
            Grammar G;
            G.readGrammarFile(filename);
            return generate(G, start);
        }
};

#endif