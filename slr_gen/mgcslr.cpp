#include <iostream>
#include "../src/cfg.hpp"
#include "../src/ParserGenerator.hpp"
#include "lr_item_set.hpp"
#include <stack>
#include "lexer.hpp"
using namespace std;

void showUsage(string name) {
    cout<<"MGCPGen (c) 2026 MaxGCoding.com"<<endl;
    cout<<name<<" <grammar file> <output file>"<<endl;
}

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

class SLRGenerator {
    private:
        using GoToTable = map<int,map<Symbol,int>>;
        using ActionTable = map<int,map<Symbol,string>>;
        unordered_set<Symbol> symbols;
        ComputeFirstSets    firsts;
        ComputeFollowSets   follows;
        DirectedGraph       cfsm;
        vector<LRState>     states;

        GoToTable make_goto_table(Grammar& G) {
            GoToTable tab;
            for (int s = 0; s < cfsm.V(); s++) {
                for (auto it = cfsm.adj(s); it != nullptr; it = it->next) {
                    //if (G.nonterminals.count(it->edgeLabel))
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
    public:
        SLRGenerator() {

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
        void generate(Grammar& G, Symbol ss) {
            firsts.compute(G);
            firsts.printFirsts(G);
            follows.compute(G, ss);
            follows.printFollows(G);
            generate_CFSM(G, ss);
            goTab = make_goto_table(G);
            actTable = make_action_table(G, ss);
            cout<<"GOTO table: "<<endl;
            for (auto e : goTab) {
                for (auto t : e.second) {
                    cout<<e.first<<": "<<t.first<<" "<<t.second<<endl;
                }
            }
            cout<<"ACTION table: "<<endl;
            for (auto e : actTable) {
                for (auto t : e.second) {
                    cout<<e.first<<": "<<t.first<<" "<<t.second<<endl;
                }
            }
        }
        ActionTable actTable;
        GoToTable goTab;
        void shift_reduce_driver(Grammar& G, vector<Token>& tokens) {
            stack<LRState> st;
            st.push(states[0]);
            int tpos = 0;
            for (;;) {
                Token T = tokens[tpos];
                LRState S = st.top();
                cout<<"["<<S.state_num<<"]["<<tokenStr[T.getSymbol()]<<"]"<<actTable[S.state_num][T.getString()]<<endl;
                switch (actTable[S.state_num][tokenStr[T.getSymbol()]][0]) {
                    case 'a': {
                        cout<<"Input parsed successfully."<<endl;
                        return;
                    } break;
                    case 's': {
                        cout<<"SHIFT"<<endl;
                        if (goTab[S.state_num].find(tokenStr[T.getSymbol()]) != goTab[S.state_num].end()) {
                            int next = goTab[S.state_num][tokenStr[T.getSymbol()]];
                            st.push(states[next]);
                        } else {
                            cout<<"Hmmm.. nothing to shift?"<<endl;
                        }
                        tpos++;
                    } break;
                    case 'r': {
                        cout<<"REDUCE"<<endl;
                        int rto = stoi(actTable[S.state_num][tokenStr[T.getSymbol()]].substr(1));
                        Production X = G.prodById[rto];
                        for (int i = 0; i < X.rhs.size(); i++)
                            st.pop();
                        st.push(states[goTab[st.top().state_num][X.lhs]]);
                    } break;
                    case 'e': {
                        cout<<"Syntax Error: "<<T.getString()<<endl;
                    } break;
                    default:
                        cout<<"Well shit."<<endl;
                        return;
                        break;
                }
            }
        }
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        showUsage(argv[0]);
        return 1;
    }
    Grammar G;
    G.readGrammarFile(argv[1]);
    SLRGenerator slrgen;
    slrgen.generate(G, "sp");
    Lexer lexer;
    StringBuffer* sb = new StringBuffer();
    sb->init("2+3*4");
    vector<Token> tokens = lexer.lex(sb);
    cout<<"Parsing: 2+3*4: "<<endl;
    slrgen.shift_reduce_driver(G, tokens);
    return 0;
}