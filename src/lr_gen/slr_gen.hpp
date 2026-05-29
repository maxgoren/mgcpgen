#ifndef slr_gen_hpp
#define slr_gen_hpp
#include <iostream>
#include <functional>
#include "../../src/cfg.hpp"
#include "../../src/calc_firsts.hpp"
#include "../../src/calc_follows.hpp"
#include "lr_item_set.hpp"
#include "directed_graph.hpp"
#include <stack>
using namespace std;

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
        GoToTable make_goto_table(Grammar& G) {
            GoToTable tab;
            for (int s = 0; s < cfsm.V(); s++) {
                for (auto it = cfsm.adj(s); it != nullptr; it = it->next) {
                    if (G.nonterminals.count(it->edgeLabel))
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
                        if (tab[s].count(it->edgeLabel)) {
                            cout<<"Shift conflict: ["<<s<<"]["<<it->edgeLabel<<"] "<<tab[s][it->edgeLabel]<<endl;
                        }
                        tab[s][it->edgeLabel] = "s"+to_string(it->dest);
                    }
                }
            }
            for (int s = 0; s < states.size(); s++) {
                for (const LRItem& item : states[s].getItems()) {
                    if (!item.complete()) continue;
                    Production p = item.production;
                    if (p.lhs == ss) {
                        tab[s]["$"] = "accept";
                        continue;
                    }
                    for (Symbol a : G.follow.at(p.lhs)) {
                        if (tab[s].count(a)) {
                            cout<<"Reduce conflict: ["<<s<<"]["<<a<<"] "<<tab[s][a]<<endl;
                        }
                        tab[s][a] = "r"+to_string(p.pid);
                    }
                }
            }
            return tab;
        }
        LRState closure(const Grammar& G, const LRState& state) {
            LRState ret;
            queue<LRItem> work;
            for (const LRItem& item : state.getItems()) {
                work.push(item);
                ret.addItem(item);
            }
            while (!work.empty()) {
                Symbol X = work.front().symbolAfterDot();
                work.pop();
                if (G.nonterminals.count(X)) {
                    for (const Production& p : G.productions.at(X)) {
                        LRItem newItem(p, 0);
                        if (!ret.hasItem(newItem)) {
                            work.push(newItem);
                            ret.addItem(newItem);
                        }
                    }
                }
            }
            return ret;
        }

        LRState lr_goto(Grammar& G, const LRState& state, Symbol X) {
            LRState next;
            for (LRItem item : state.getItems()) {
                if (item.symbolAfterDot() == X) {
                    next.addItem(item.advance());
                }
            }
            return closure(G, next);
        }
        void generate_CFSM(Grammar& G, Symbol ss) {
            for (Symbol s : G.terminals)
                symbols.insert(s);
            for (Symbol s : G.nonterminals)
                symbols.insert(s);
            LRState start;
            start.addItem(LRItem(G.productions[ss][0], 0));
            LRState I0 = closure(G, start);
            queue<LRState> fq;
            unordered_set<string> seen;   
            I0.setStateNum(0);
            fq.push(I0);
            seen.insert(I0.key());
            states.push_back(I0);
            while (!fq.empty()) {
                LRState curr = fq.front(); fq.pop();
                unordered_set<Symbol> valid;
                if (debug_noise) {
                    cout<<"Current state(I"<<curr.getStateNum()<<"): \n";
                    cout<<curr.key()<<endl;
                }
                for (auto item : curr.getItems()) {
                    Symbol tmp = item.symbolAfterDot();
                    if (tmp != "<fin>")
                        valid.insert(tmp);
                }
                for (auto X : valid) {
                    LRState gt = lr_goto(G, curr, X);
                    if (gt.getItems().empty())
                        continue;
                    int target;
                    if (seen.find(gt.key()) == seen.end()) {
                        gt.setStateNum(states.size());
                        fq.push(gt);
                        states.push_back(gt);
                        seen.insert(gt.key());
                        target = gt.getStateNum();
                    } else {
                        for (auto s : states) {
                            if (gt.key() == s.key()) {
                                target = s.getStateNum();
                                break;
                            }
                        }
                    }
                    cfsm.addEdge(curr.getStateNum(), target, X);   
                    if (debug_noise) {
                        cout<<"Add edge from I"<<curr.getStateNum()<<" on "<<X<<" to I"<<target<<endl;                 
                    }
                }
                if (debug_noise) {
                    cout<<"--------------------------\n";
                }
            }
        }
        void printPrelude(ostream& ofile) {
            ofile<<"#include <vector>\n";
            ofile<<"#include <map>\n";
            ofile<<"#include <set>\n";
            ofile<<"#include \"production.hpp\"\n";
            ofile<<"using namespace std; \n";
        }
        void printProductions(ostream& os, Grammar& G, string name) {
            os<<"enum NTSYMBOL {\n";
            int i = 0;
            for (auto t : G.nonterminals) {
                if (t != "#") {
                    os<<t;
                    if (i+1 < G.nonterminals.size())
                        os<<", ";
                    if (i % 5 == 0) 
                        os<<endl;
                }
                i++;
            }
            os<<"\n};\n";
            os<<"map<int, Production> "<<name<<";"<<endl;
            os<<"void init"<<name<<"() {\n";
            for (auto e : G.prodById) {
                os<<"\t prod["<<e.first<<"]  = Production("<<e.second.pid<<",\""<<e.second.lhs<<"\", ";
                os<<"SymbolString(";
                if (e.second.rhs.size() > 0) {
                    os<<"{";
                    for (int i = 0; i < e.second.rhs.size(); i++) {
                        os<<"\""<<e.second.rhs[i]<<"\"";
                        if (i+1 < e.second.rhs.size())
                            os<<",";
                    }
                    os<<"}";
                }
                os<<"),\""<<e.second.action<<"\");"<<endl;
            }
            os<<"}\n";
        }
        template <class Iterable>
        void printTables(ostream& os, Iterable table, string tableName) {
            os<<"map<int,map<string,string>> "<<tableName<<";\n";
            os<<"void init"<<tableName<<"() {\n";
            for (auto e : table) {
                os<<"\t "<<tableName<<"["<<e.first<<"] = {";
                int i = 0;
                for (auto t : e.second) {
                    os<<"{\""<<t.first<<"\", \""<<t.second<<"\"}";
                    if (i+1 < e.second.size())
                        os<<", ";
                    i++;
                }
                os<<"};"<<endl;
            }
            os<<"}"<<endl;
        }
        pair<ActionTable, GoToTable> generate(Grammar& G, Symbol ss, ofstream& ofile) {
            firsts.compute(G);
            follows.compute(G, ss);
            generate_CFSM(G, ss);
            ActionTable actTable;
            GoToTable goTab;                                                                                                                                                                               
            goTab = make_goto_table(G);
            actTable = make_action_table(G, ss);
            printPrelude(ofile);
            printProductions(ofile, G, "prod");
            printTables(ofile, goTab, "goTab");
            printTables(ofile, actTable, "actTab");
            return make_pair(actTable, goTab);
        }
    public:
        SLRGenerator() {

        }
        vector<LRState>& getStates() {
            return states;
        }
        pair<ActionTable,GoToTable> generate(Grammar& G,  string outname, Symbol start) {
            ofstream ofile(outname);
            auto ret = generate(G, start, ofile);
            ofile.close();
            return ret;
        }
        pair<ActionTable,GoToTable> generate(string filename, string outname,Symbol start) {
            Grammar G;
            G.readGrammarFile(filename);
            ofstream ofile(outname);
            auto ret = generate(G, start, ofile);
            ofile.close();
            return ret;
        }
};

#endif