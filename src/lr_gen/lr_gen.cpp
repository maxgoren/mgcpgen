#include "lr_gen.hpp"
#include "serialize.hpp"
#include <bitset>
using std::bitset;
//For handling shift/reduce conflicts via precedence rules. 
// 1) First we check to see if production has an overriding symbol.
// 2) failing to find that a production inherits the precedence of  it's rightmost terminal. 
//  if there are no terminals we return the empty string.
Symbol LRGenerator::get_production_precedence_symbol(const Production& p, const Grammar& G) {
    cout<<"Production prec symbol: ";
    if (p.precOverride != -1) {
        cout<<" (override)"<<p.precOverride<<endl;
        return p.precOverride;
    }
    //productions inherit the precedence of the _right_ most terminal symbol
    //as such, we scan the production _backwards_ so we can exit on the first terminal encountered.
    for (auto it = p.rhs.rbegin(); it != p.rhs.rend(); ++it) {
        if (G.terminals.count(*it)) {
            cout<<*it<<endl;
            return *it;
        }
    }
    cout<<"None."<<endl;
    return -1;
}

string LRGenerator::resolve_with_precedence(Grammar& G, ActionTable& tab, Production& p, int s, Symbol a) {
    string existing = tab[s][a];
    if (existing[0] == 's') {
        Symbol prod_sym = get_production_precedence_symbol(p, G);
        if (!G.precedenceMap.count(prod_sym) || !G.precedenceMap.count(a)) {
            cout<<"!!! No way to resolve Shift/Reduce conflict: ["<<s<<"]["<<a<<"] "<<existing<<endl;
        } else {
            OpPrec pr = G.precedenceMap[prod_sym];
            OpPrec la = G.precedenceMap[a];
            cout<<"Resolved Shift/Reduce conflict: ["<<s<<"]["<<a<<"] "<<existing<<": ";
            if (la.prec_level > pr.prec_level) {
                cout<<"Keeping existing shift"<<endl;
            } else if (pr.prec_level > la.prec_level) {
                cout<<"Made to reduce on precedence for "<<a<<endl;
                return "r" + to_string(p.pid);
            } else {
                if (la.assoc == "left") {
                    cout<<"Reduced on associativity"<<endl;
                    return "r" + to_string(p.pid);
                } else {
                    cout<<"Keeping existing shift."<<endl;
                }
            }
        }
    } else {
        string curr =  "r"+to_string(p.pid);
        if (tab[s][a] != curr) {
            cout<<"Reduce/Reduce conflict: ["<<s<<"]["<<a<<"] "<<existing<<endl;
            cout << "LALR Reduce/Reduce Conflict in State " << s << " on lookahead token '" << a << "':" << endl;
            cout << "  -> Cannot decide between " << tab[s][a] << " and r" << p.pid << endl;
            cout << "  State Items:" << endl;
            cout<<states[s].key()<<endl; 
        }
    }
    return existing;
}

GoToTable LRGenerator::make_goto_table(Grammar& G) {
    GoToTable tab;
    for (int s = 0; s < cfsm.V(); s++) {
        for (auto it : cfsm.adj(s)) {
            if (G.nonterminals.count(it.edgeLabel))
                tab[s][it.edgeLabel] = to_string(it.dest);
        }
    }
    return tab;
}

ActionTable LRGenerator::make_action_table(Grammar& G, Symbol ss) {
    ActionTable tab;
    for (int s = 0; s < cfsm.V(); s++) {
        for (auto it : cfsm.adj(s)) {
            if (G.terminals.count(it.edgeLabel)) {
                if (tab[s].count(it.edgeLabel)) {
                    cout<<"Shift conflict: ["<<s<<"]["<<it.edgeLabel<<"] "<<tab[s][it.edgeLabel]<<endl;
                } else {
                    tab[s][it.edgeLabel] = "s"+to_string(it.dest);
                }
            }
        }
    }
    for (int s = 0; s < states.size(); s++) {
        for (const LRItem& item : states[s].getItems()) {
            if (!item.complete()) continue;
            Production p = item.getProduction();
            for (int a = 0; a < G.symMap.size(); a++) {
                if (item.lookaheads().test(a)) {
                    Symbol targetToken = (a == G.symMap.get("$")) ? G.symMap.get("TK_EOI"):a;
                    if (p.lhs == ss && a == G.symMap.get("$")) {
                        tab[s][G.symMap.get("$")] = "accept";
                    } else {
                        if (!tab[s].count(targetToken)) {
                            tab[s][targetToken] = "r"+to_string(p.pid);
                        } else {
                            tab[s][targetToken] = resolve_with_precedence(G, tab, p, s, targetToken);
                        }
                    }
                }
            }
        }
    }
    return tab;
}

LRState LRGenerator::closure(Grammar& G, const LRState& state) {
    LRState ret;
    queue<LRItem> work;
    bitset<256> betaFirst;
    for (const LRItem& i : state.getItems()) {
        work.push(i);
        ret.addItem(i);
    }
    while (!work.empty()) {
        LRItem item = work.front();
        Symbol X = item.symbolAfterDot();
        work.pop();
        if (G.nonterminals.count(X)) {
            betaFirst.reset();
            betaFirst = firstFromSequence(G, item.betaSymbols());
            if (betaFirst.test(G.symMap.get(EPS))) {
                betaFirst.reset(G.symMap.get(EPS));
                betaFirst |= item.lookaheads();
            }
            if (G.productions.find(X) != G.productions.end()) {
                for (const Production& p : G.productions.at(X)) {
                    LRItem newItem(p, 0);
                    newItem.lookaheads() |= betaFirst;
                    newItem.rehash();
                    if (!ret.hasItem(newItem)) {
                        work.push(newItem);
                        ret.addItem(newItem);
                    }
                }
            } else {
                cout<<"\n Encountered Invalid Non Terminal. Bailing out."<<endl;
                exit(EXIT_FAILURE);
            }
        }
    }
    return ret;
}

LRState LRGenerator::lr_goto(Grammar& G, const LRState& state, Symbol X) {
    LRState next;
    for (LRItem item : state.getItems()) {
        if (!item.complete() && item.symbolAfterDot() == X) {
            LRItem nextItem = item.advance();
            nextItem.lookaheads() |= item.lookaheads();
            nextItem.rehash();
            next.addItem(nextItem);
        }
    }
    return closure(G, next);
}

bitset<256> LRGenerator::firstFromSequence(Grammar& G, SymbolString seq) {
    bitset<256> result;
    bool all_nullable = true;
    for (Symbol X : seq) {
        if (G.terminals.count(X)) {
            result.set(X);
            all_nullable = false;
            break;
        }
        if (G.nonterminals.count(X)) {
            for (auto s : G.firsts.at(X)) {
                result.set(s);
            }
            if (result.test(G.symMap.get(EPS)) == false) {
                all_nullable = false;
                break;
            }
        }
    }
    if (all_nullable)
        result.set(G.symMap.get(EPS));
    return result;
} 


string& LRGenerator::getKey(const LRState& st) const {
    return PARSER_TYPE == CLR ? st.key():st.coreKey();
}

void LRGenerator::generate_CFSM(Grammar& G, Symbol ss) {
    int created = 1, merged = 0;
    LRState start;
    queue<int> fq;
    unordered_map<string,int> seen;
    LRItem first_item(G.productions[ss][0], 0); 
    first_item.lookaheads().set(G.symMap.get("$"));  
    start.addItem(first_item);
    LRState I0 = closure(G, start);
    I0.setStateNum(0);
    states.push_back(I0);
    fq.push(0);
    seen.insert({getKey(I0),I0.getStateNum()});
    while (!fq.empty()) {
        int stateIdx = fq.front(); fq.pop();
        unordered_set<Symbol> valid;
        for (auto& item : states[stateIdx].getItems()) {
            Symbol tmp = item.symbolAfterDot();
            if (tmp != -1)
                valid.insert(tmp);
        }
        if (debug_noise) {
            cout<<"Current state: I"<<stateIdx<<" \n";
        }
        for (auto const & X : valid) {
            LRState gt = lr_goto(G, states[stateIdx], X);
            if (gt.getItems().empty())
                continue;
            int target;
            string gt_key = getKey(gt);
            auto seenIt = seen.find(gt_key);
            if (seenIt == seen.end()) {
                int ns_num = states.size();
                gt.setStateNum(ns_num);
                fq.push(ns_num);
                states.push_back(gt);
                seen.insert({gt_key,ns_num});
                target = ns_num;
                if (debug_noise) {
                    cout<<"\t + Created new state: I"<<gt.getStateNum()<<"\n";
                }
                created++;
            } else {
                target = seen[gt_key];
                if (PARSER_TYPE == LALR && states[target].mergeLookaheadsFrom(gt)) {
                    fq.push(target); 
                    if (debug_noise) {
                        cout<<"\t - Existing state: I"<<target<<" merged lookaheads"<<endl;
                    }
                    
                    merged++;
                }
            }
            cout<<"\r States Created: "<<created<<", Merged: "<<merged<<flush;
            if (!cfsm.hasEdge(stateIdx, target, X)) {
                cfsm.addEdge(stateIdx, target, X);
            }
        }
        if (debug_noise) {
            cout<<"--------------------------\n";
        }
    }
    cout<<endl;
}

pair<ActionTable, GoToTable> LRGenerator::generate(Grammar& G, Symbol ss, ofstream& ofile) {
    NullableCalculator    nullable;
    FirstSetCalculator    firsts;
    FollowSetCalculator   follows;
    FileWriter            fileWriter;
    cout<<"[*] Analyzing Context Free Grammar: "<<endl;
    cout<<"\t (1) Calculating Nullable set... \n";
    nullable.compute(G);
    cout<<"\t (1) Done.\n\t (2) Calculating Firsts set...   \n";
    firsts.compute(G);
    cout<<"\t (2) Done.\n\t (3) Calculating Follows set...  \n";
    follows.compute(G, ss);
    cout<<"\t (3) Done."<<endl; 
    cout<<"[*] Building "<<(PARSER_TYPE == CLR ? "CLR":"LALR")<<" DFA"<<endl;
    generate_CFSM(G, ss);
    cout<<"[*] Completed with "<<states.size()<<" LR states and "<<cfsm.E()<<" edges."<<endl;
    cout<<"[*] Generating Go To table"<<endl;
    GoToTable   goTab = make_goto_table(G);
    cout<<"[*] Generating Action table"<<endl;
    ActionTable actTab = make_action_table(G, ss);
    fileWriter.write(ofile, G, cfsm, goTab, actTab);
    return make_pair(actTab, goTab);
}

LRGenerator::LRGenerator(bool noise, ParserType pt) {
    debug_noise = noise;
    PARSER_TYPE = pt;
}

vector<LRState>& LRGenerator::getStates() {
    return states;
}


pair<ActionTable,GoToTable> LRGenerator::generate(Grammar& G, string outname) {
    ofstream ofile(outname);
    cout<<"[*] Generating Parser..."<<endl;
    auto ret = generate(G, G.startSym, ofile);
    ofile.close();
    cout<<"[*] Done."<<endl;
    return ret;
}