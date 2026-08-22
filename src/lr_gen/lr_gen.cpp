#include "lr_gen.hpp"
//For handling shift/reduce conflicts via precedence rules. 
// 1) First we check to see if production has an overriding symbol.
// 2) failing to find that a production inherits the precedence of  it's rightmost terminal. 
//  if there are no terminals we return the empty string.
Symbol LRGenerator::get_production_precedence_symbol(const Production& p, const Grammar& G) {
    cout<<"Production prec symbol: ";
    if (!p.precOverride.empty()) {
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
    return "";
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
                tab[s][it.edgeLabel] = it.dest;
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
            for (Symbol a : item.lookaheads()) {
                Symbol targetToken = (a == "$") ? "TK_EOI":a;
                if (p.lhs == ss && a == "$") {
                    tab[s]["$"] = "accept";
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
    return tab;
}

LRState LRGenerator::closure(const Grammar& G, const LRState& state) {
    LRState ret;
    queue<LRItem> work;
    for (const LRItem& item : state.getItems()) {
        work.push(item);
        ret.addItem(item);
    }
    while (!work.empty()) {
        LRItem item = work.front();
        Symbol X = item.symbolAfterDot();
        work.pop();
        if (G.nonterminals.count(X)) {
            unordered_set<Symbol> betaFirst = firstFromSequence(G, item.betaSymbols());
            if (betaFirst.count(EPS)) {
                betaFirst.erase(EPS);
                betaFirst.insert(item.lookaheads().begin(), item.lookaheads().end());
            }
            if (G.productions.find(X) != G.productions.end()) {
            for (const Production& p : G.productions.at(X)) {
                LRItem newItem(p, 0);
                newItem.lookaheads().insert(betaFirst.begin(), betaFirst.end());
                newItem.rehash();
                if (!ret.hasItem(newItem)) {
                    work.push(newItem);
                    ret.addItem(newItem);
                }
            }
            } else {
                cout<<"\n Hey, some thing funky with "<<X<<" is going on."<<endl;
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
            nextItem.lookaheads().insert(item.lookaheads().begin(), item.lookaheads().end());
            nextItem.rehash();
            next.addItem(nextItem);
        }
    }
    return closure(G, next);
}

unordered_set<Symbol> LRGenerator::firstFromSequence(const Grammar& G, SymbolString seq) {
    unordered_set<Symbol> result;
    for (Symbol X : seq) {
        if (G.terminals.count(X)) {
            result.insert(X);
            return result;
        }
        if (G.nonterminals.count(X)) {
            result.insert(G.firsts.at(X).begin(), G.firsts.at(X).end());
            if (result.find(EPS) == result.end())
                return result;
        }
    }
    result.insert(EPS);
    return result;
} 


string& LRGenerator::getKey(const LRState& st) const {
    return PARSER_TYPE == CLR ? st.key():st.coreKey();
}

void LRGenerator::generate_CFSM(Grammar& G, Symbol ss) {
    LRState start;
    queue<int> fq;
    unordered_map<string,int> seen;
    LRItem first_item(G.productions[ss][0], 0); 
    first_item.lookaheads().insert("$");  
    start.addItem(first_item);
    LRState I0 = closure(G, start);
    I0.setStateNum(0);
    fq.push(0);
    seen.insert({getKey(I0),I0.getStateNum()});
    states.push_back(I0);
    while (!fq.empty()) {
        LRState curr = states[fq.front()]; fq.pop();
        unordered_set<Symbol> valid;
        for (auto item : curr.getItems()) {
            Symbol tmp = item.symbolAfterDot();
            if (tmp != "<fin>")
                valid.insert(tmp);
        }
        cout<<"Current state: I"<<curr.getStateNum()<<" \n";
        for (auto X : valid) {
            LRState gt = lr_goto(G, curr, X);
            if (gt.getItems().empty())
                continue;
            int target;
            if (seen.find(getKey(gt)) == seen.end()) {
                gt.setStateNum(states.size());
                fq.push(gt.getStateNum());
                states.push_back(gt);
                seen.insert({getKey(gt),gt.getStateNum()});
                target = gt.getStateNum();
                cout<<"\t + Created new state: I"<<gt.getStateNum()<<"\n";
            } else {
                target = seen[getKey(gt)];
                if (PARSER_TYPE == LALR && states[target].mergeLookaheadsFrom(gt)) {
                    fq.push(states[target].getStateNum()); 
                    cout<<"\t - Existing state: I"<<target<<" merged lookaheads"<<endl;
                }
            }
            if (!cfsm.hasEdge(curr.getStateNum(), target, X)) {
                cfsm.addEdge(curr.getStateNum(), target, X);   
                cout<<"\t + Add edge from I"<<curr.getStateNum()<<" on "<<X<<" to I"<<target<<endl;                 
            }
        }
        if (debug_noise) {
            cout<<"--------------------------\n";
        }
    }
    cout<<"\n";
}

void LRGenerator::printPrelude(ostream& ofile) {
    ofile<<"#include <vector>\n";
    ofile<<"#include <map>\n";
    ofile<<"#include <set>\n";
    ofile<<"#include <functional>\n";
    ofile<<"#include \"production.hpp\"\n";
    ofile<<"using namespace std; \n";
}
void LRGenerator::printProductions(ostream& os, Grammar& G, string name) {
    os<<"enum NTSYMBOL {\n";
    int i = 0;
    for (auto t : G.nonterminals) {
        if (t != "#" && !t.empty()) {
            os<<t;
            if (i+1 < G.nonterminals.size())
                os<<", ";
            if (i > 1 && i % 5 == 0) 
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
void LRGenerator::printTables(ostream& os, Iterable table, string tableName) {
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

void LRGenerator::printActionRegistrar(ostream& os, Grammar& G) {
    os<<"map<string, function<AST*(vector<AST*>&)>> actions;\n";
    os<<"void initActions() {\n";
    for (auto actions : G.actionMap) {
        os<<"\t actions.insert({\""<<actions.first<<"\","<<actions.second<<"});\n";
    }
    os<<"}"<<endl;
}

pair<ActionTable, GoToTable> LRGenerator::generate(Grammar& G, Symbol ss, ofstream& ofile) {
    CalculateNullable         nullable;
    FirstSetCalculator    firsts;
    FollowSetCalculator   follows;
    cout<<"[*] Analyzing Context Free Grammar: "<<endl;
    cout<<"\t (1) Calculating Nullable set... \n";
    nullable.compute(G);
    cout<<"\t (1) Done.\n\t (2) Calculating Firsts set...   \n";
    firsts.compute(G);
    cout<<"\t (2) Done.\n\t (3) Calculating Follows set...  \n";
    follows.compute(G, ss);
    cout<<"\t (3) Done.\n[*] Building "<<(PARSER_TYPE == CLR ? "CLR":"LALR")<<" NFA"<<endl;
    generate_CFSM(G, ss);
    cout<<"[*] Completed with "<<states.size()<<" LR states and "<<cfsm.E()<<" edges."<<endl;
    cout<<"[*] Generating Go To table"<<endl;
    GoToTable   goTab = make_goto_table(G);
    cout<<"[*] Generating Action table"<<endl;
    ActionTable actTable = make_action_table(G, ss);
    printPrelude(ofile);
    printProductions(ofile, G, "prod");
    printTables(ofile, goTab, "goTab");
    printTables(ofile, actTable, "actTab");
    printActionRegistrar(ofile, G);
    return make_pair(actTable, goTab);
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