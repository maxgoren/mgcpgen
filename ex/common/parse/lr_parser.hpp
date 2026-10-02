#ifndef lr_parser_hpp
#define lr_parser_hpp
#include <iostream>
#include <functional>
#include <algorithm>
#include <stack>
#include "actions.hpp"
#include "mgcpgen_out.hpp"
#include "ast.hpp"
using namespace std;

class LRParser {
    private:
        stack<AST*> semStack;
        stack<int> st;
        int tpos;
        vector<Token> tokens;
        bool debug_noise;
        Token& current() {
            return tokens[tpos];
        }
        void advance() {
            if (tpos < tokens.size()) {
                tpos++;
            }
        }
        int nextState(const int *table[], int state, int sym) {
            int N = table[state][0];
            for (int i = 1; i < 2*N+1; i+=2) {
                if (table[state][i] == sym || sym == TK_EOI && table[state][i] == DOLLARACCEPT) {
                    return i+1;
                }
            }
            return -1;
        }
        void doShift(int next) {
            if (debug_noise)
                cout<<"SHIFT "<<current().getString()<<endl;
            st.push(next);
            semStack.push(new AST(current()));
            advance();
        }
        void doReduce(int next) {
            if (debug_noise)
                cout<<"REDUCE "<<endl;
            Production X = prod[abs(next)];
            vector<AST*> tmp;
            for (int i = 0; i < X.rhs.size(); i++) {
                st.pop();
                if (!semStack.empty()) {
                    auto m = semStack.top();
                    if (m->token.getString() != "<nil>") {
                        tmp.push_back(semStack.top());
                    } else {
                        delete m;
                    }
                    semStack.pop();
                } else {
                    cout<<"Uh oh: Semantic Stack and Parse Stack have diverged"<<endl;
                }
            }
            reverse(tmp.begin(), tmp.end());
            if (X.actsym.empty() == false) {
                if (debug_noise) cout<<"And do: "<<X.actsym<<endl;
                string f = X.actsym.substr(1);
                semStack.push(actions.at(f)(tmp));
                if (debug_noise)
                    preorder(semStack.top(), 1);
            } else {
                if (X.rhs.empty()) {
                    semStack.push(new AST(Token(TK_EOI, "Epsilon")));
                } else {
                    semStack.push(tmp.front());
                }
            }
            int ns = nextState(goTab, st.top(), X.lhs);
            if (ns != -1) {
                st.push(goTab[st.top()][ns]);
            }
        }
        void printCurrent(int state_num, Token& T) {
            cout<<"[ state: "<<state_num<<"][ token: "<<tokenStr[T.getSymbol()]<<"]"<<actTab[state_num][nextState(actTab, state_num,T.getSymbol())]<<endl<<"Action: ";
        }
    public:
        LRParser(bool loud = true) {
            debug_noise = loud;
        }
        AST* parse(vector<Token>& tok) {
            tokens = tok;
            tpos = 0;
            st.push(0);
            for (;;) {
                Token curr_token = current();
                int curr_state = st.top();
                int ns = nextState(actTab, curr_state, curr_token.getSymbol());
                if (ns == -1) {
                    cout<<"Hmm, no actions on '"<<tokenStr[curr_token.getSymbol()]<<"' from state "<<curr_state<<"?"<<endl;
                    int nument = 2*actTab[curr_state][0]+1;
                    for (int i = 1; i < nument; i+=2) {
                        cout<<actTab[curr_state][i]<<endl;
                    }
                    cout<<"Bailing out."<<endl;
                    return nullptr;
                } else {
                    if (debug_noise)
                        printCurrent(curr_state, curr_token);
                    int next = actTab[curr_state][ns];
                    if (next > 0) {
                        doShift(next);
                    } else if (next < 0) {
                        doReduce(next);
                    } else {
                        return semStack.top();
                    }
                }
            }
            return nullptr;
        }
    };

#endif