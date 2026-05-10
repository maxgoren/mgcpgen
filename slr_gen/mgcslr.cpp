#include <iostream>
#include <functional>
#include "../src/cfg.hpp"
#include "../src/calc_firsts.hpp"
#include "../src/calc_follows.hpp"
#include "lr_item_set.hpp"
#include <stack>
#include "lexer.hpp"
using namespace std;


/*

        }
*/

void showUsage(string name) {
    cout<<"MGCPGen (c) 2026 MaxGCoding.com"<<endl;
    cout<<name<<" <grammar file> <output file>"<<endl;
}

struct AST {
    Token token;
    vector<AST*> children;
    AST(Token& tk) : token(tk), children(vector<AST*>(3)) { }
    AST() { }
};

void preorder(AST* ast, int d) {
    if (ast != nullptr) {
        for (int i = 0; i < d; i++) cout<<" ";
        cout<<ast->token.getString()<<endl;
        for (AST* n : ast->children) {
            preorder(n, d+1);
        }
    }
}

double eval(AST* ast) {
    if (ast != nullptr) {
        if (ast->children[0] == nullptr) {
            cout<<"Leaf Node: "<<ast->token.getString()<<endl;
            if (ast->token.getSymbol() == TK_NUM) {
                return stod(ast->token.getString());
            } else {
                return 0;
            }
        } else {
            double lhs = eval(ast->children[0]);
            double rhs = eval(ast->children[1]);
            switch (ast->token.getSymbol()) {
                case TK_MINUS: {
                    if (ast->children[1] == nullptr) {
                        return -lhs;
                    } else {
                        return lhs-rhs;
                    }
                } break;
                case TK_PLUS: return lhs+rhs;
                case TK_MUL:  return lhs*rhs;
                case TK_DIV:  return lhs/rhs;
                case TK_NUM:  return stod(ast->token.getString());
            }
        }
    }
    return 0;
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

struct ReducingFrame {
    Symbol sym;
    AST* node;
    ReducingFrame(Symbol s = "<nil>", AST* n = nullptr) : sym(s), node(n) { }
};

AST* makebinop(vector<ReducingFrame>& reducing) {
    AST* nn = reducing[1].node;
    nn->children[0] = reducing[0].node;
    nn->children[1] = reducing[2].node;
    return nn; 
}
AST* makeunary(vector<ReducingFrame>& reducing) {
    AST* nn = new AST(reducing[0].node->token);
    nn->children[0] = reducing[1].node;
    return nn;
}

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
    public:
        SLRGenerator() {

        }
        vector<LRState>& getStates() {
            return states;
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
        pair<ActionTable, GoToTable> generate(Grammar& G, Symbol ss) {
            firsts.compute(G);
            firsts.printFirsts(G);
            follows.compute(G, ss);
            follows.printFollows(G);
            generate_CFSM(G, ss);
            ActionTable actTable;
            GoToTable goTab;
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
            return make_pair(actTable, goTab);
        }
        
};


class SLRParser {
    private:
        ActionTable actTable;
        GoToTable goTab;
        vector<LRState> states;
        map<string, function<AST*(vector<ReducingFrame>&)>> actions;
    public:
        SLRParser(ActionTable at, GoToTable gt, vector<LRState>& st) {
                actions.insert(make_pair("binop", [](auto& a) { return makebinop(a); }));
                actions.insert(make_pair("unary", [](auto& a) { return makeunary(a); }));
                actions.insert(make_pair("num", [](auto& a) { return a[0].node; }));
                actions.emplace("id", [](auto& a) { return a[0].node; });
                actions.emplace("pass", [](auto& a) { return a[1].node; });
            actTable = at;
            goTab = gt;
            states = st;
        }
        void shift_reduce_driver(Grammar& G, vector<Token>& tokens) {
            stack<LRState> st;
            stack<AST*> semStack;
            st.push(states[0]);
            int tpos = 0;
            for (;;) {
                Token T = tokens[tpos];
                LRState S = st.top();
                cout<<"[ state: "<<S.state_num<<"][ token: "<<tokenStr[T.getSymbol()]<<"]"<<actTable[S.state_num][T.getString()]<<endl<<"Action: ";
                if (T.getSymbol() == TK_EOI && actTable[S.state_num]["$"] == "accept") {
                    cout<<"ACCEPT"<<endl;
                    preorder(semStack.top(), 1);
                    cout<<eval(semStack.top())<<endl;
                    return;
                }
                switch (actTable[S.state_num][tokenStr[T.getSymbol()]][0]) {
                    case 's': {
                        cout<<"SHIFT"<<endl;
                        if (goTab[S.state_num].find(tokenStr[T.getSymbol()]) != goTab[S.state_num].end()) {
                            int next = goTab[S.state_num][tokenStr[T.getSymbol()]];
                            st.push(states[next]);
                        } else {
                            cout<<"Hmmm.. nothing to shift?"<<endl;
                        }
                        semStack.push(new AST(T));
                        tpos++;
                    } break;
                    case 'r': {
                        cout<<"REDUCE"<<endl;
                        int rto = stoi(actTable[S.state_num][tokenStr[T.getSymbol()]].substr(1));
                        Production X = G.prodById[rto];
                        vector<ReducingFrame> tmp;
                        for (int i = 0; i < X.rhs.size(); i++) {
                            st.pop();
                            if (!semStack.empty()) {
                                tmp.push_back(ReducingFrame(X.rhs[i], semStack.top()));
                                semStack.pop();
                            } else {
                                tmp.push_back(ReducingFrame(X.rhs[i], nullptr));
                            }
                        }
                        reverse(tmp.begin(), tmp.end());
                        cout<<"And do: ";
                        if (X.action.empty() == false) {
                            cout<<X.action<<endl;
                            semStack.push(actions[X.action.substr(1)](tmp));
                        } else {
                            for (auto m : tmp) {
                                if (m.node != nullptr)
                                    semStack.push(m.node);
                            }
                        }
                        st.push(states[goTab[st.top().state_num][X.lhs]]);
                    } break;
                    default:
                        cout<<"Well shit. Syntax Error: "<<T.getSymbol()<<endl;
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
    auto[actTab, goTab] = slrgen.generate(G, "sp");
    Lexer lexer;
    string expr = "-2*3+4";
    do {
    StringBuffer* sb = new StringBuffer();
    sb->init(expr);
    vector<Token> tokens = lexer.lex(sb);
    cout<<"Parsing: "<<expr<<endl;

    SLRParser parser(actTab, goTab, slrgen.getStates());
    parser.shift_reduce_driver(G, tokens);
    cout<<"?> ";
    getline(cin, expr);
    } while (true);
    return 0;
}