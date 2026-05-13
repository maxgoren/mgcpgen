#ifndef slr_parser_hpp
#define slr_parser_hpp
#include <iostream>
#include <functional>
#include "../src/cfg.hpp"
#include "../src/calc_firsts.hpp"
#include "../src/calc_follows.hpp"
#include "lr_item_set.hpp"
#include <stack>
#include "lexer.hpp"
#include "ex/actions.hpp"
using namespace std;

class SLRParser {
    private:
        ActionTable actTable;
        GoToTable goTab;
        vector<LRState> states;
        map<string, function<AST*(vector<AST*>&)>> actions;
        int tpos;
        vector<Token> tokens;
        Token& current() {
            return tokens[tpos];
        }
        void advance() {
            if (tpos < tokens.size()) {
                tpos++;
            }
        }
    public:
        SLRParser(ActionTable at, GoToTable gt, vector<LRState>& st) {
            actions.insert(make_pair("binop", [](auto& a) { return makebinop(a); }));
            actions.insert(make_pair("unary", [](auto& a) { return makeunary(a); }));
            actions.insert(make_pair("num", [](auto& a) { a[0]->type = NUM_EXPR; return a[0]; }));
            actions.insert(make_pair("id", [](auto& a) { a[0]->type = ID_EXPR; return a[0]; }));
            actions.insert(make_pair("string", [](auto& a) { a[0]->type = STRING_EXPR; return a[0]; }));
            actions.insert(make_pair("pass", [](auto& a) { return a[1]; }));
            actions.insert(make_pair("mklist", [](auto& a) { return makelist(a); }));
            actions.insert(make_pair("mkprint", [](auto& a) { return makeprint(a); }));
            actions.insert(make_pair("mkif", [](auto& a) { return makeIf(a); }));
            actions.insert(make_pair("mkblock", [](auto& a) { return makeBlock(a); }));
            actions.insert(make_pair("mkfunc", [](auto& a) { return makeFunc(a); }));
            actions.insert(make_pair("mkcall", [](auto& a) { return makeCall(a); }));
            actions.insert(make_pair("mklet", [](auto& a) { return makeLet(a); }));
            actions.insert(make_pair("mkret", [](auto& a) { return makeRet(a); }));
            actions.insert(make_pair("mkexprstmt", [](auto& a) { return a[0]; }));
            actTable = at;
            goTab = gt;
            states = st;
        }
        void doShift(stack<LRState>& st, stack<AST*>& semStack, LRState S, Token& T) {
            cout<<"SHIFT"<<endl;
            if (goTab[S.state_num].find(tokenStr[T.getSymbol()]) != goTab[S.state_num].end()) {
                int next = goTab[S.state_num][tokenStr[T.getSymbol()]];
                st.push(states[next]);
            } else {
                cout<<"Hmmm.. nothing to shift?"<<endl;
            }
            semStack.push(new AST(current()));
            advance();
        }
        void doReduce(Production& X, LRState S, Token& T, stack<LRState>& st, stack<AST*>& semStack) {
            cout<<"REDUCE"<<endl;
            int rto = stoi(actTable[S.state_num][tokenStr[T.getSymbol()]].substr(1));
            vector<AST*> tmp;
            for (int i = 0; i < X.rhs.size(); i++) {
                st.pop();
                tmp.push_back(semStack.top());
                semStack.pop();
            }
            reverse(tmp.begin(), tmp.end());
            if (X.action.empty() == false) {
                cout<<"And do: ";
                cout<<X.action<<endl;
                semStack.push(actions[X.action.substr(1)](tmp));
            } else {
                for (auto m : tmp) {
                    if (m != nullptr)
                        semStack.push(m);
                }
            }
            st.push(states[goTab[st.top().state_num][X.lhs]]);
            preorder(semStack.top(), 1);
        }
        void printCurrent(LRState& S, Token& T) {
            //cout<<S.key()<<endl;
            cout<<"[ state: "<<S.state_num<<"][ token: "<<tokenStr[T.getSymbol()]<<"]"<<actTable[S.state_num][T.getString()]<<endl<<"Action: ";
        }
        bool checkAccept(LRState& S, Token& T) {
            if (T.getSymbol() == TK_EOI && actTable[S.state_num]["$"] == "accept") {
                cout<<"ACCEPT"<<endl;
                return true;
            }
            return false;
        }
        AST* shift_reduce_driver(Grammar& G, vector<Token>& tok) {
            tokens = tok;
            tpos = 0;
            stack<LRState> st;
            stack<AST*> semStack;
            st.push(states[0]);
            for (;;) {
                Token T = current();
                LRState S = st.top();
                printCurrent(S, T);
                if (checkAccept(S, T))
                    return semStack.top();
                string next = actTable[S.state_num][tokenStr[T.getSymbol()]];
                switch (next[0]) {
                    case 's': {
                        doShift(st, semStack, S, T);
                    } break;
                    case 'r': {
                        int rto = stoi(next.substr(1));
                        Production X = G.prodById[rto];
                        doReduce(X, S, T, st, semStack);
                    } break;
                    default:
                        cout<<"Syntax Error: "<<tokenStr[T.getSymbol()]<<endl;
                        return nullptr;
                }
            }
            return nullptr;
        }
    };

#endif