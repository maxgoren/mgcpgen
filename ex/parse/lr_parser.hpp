#ifndef lr_parser_hpp
#define lr_parser_hpp
#include <iostream>
#include <functional>
#include <stack>
#include "actions.hpp"
#include "mgcpgen_out.hpp"
using namespace std;

class LRParser {
    private:
        stack<AST*> semStack;
        stack<int> st;
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
        LRParser() {
            initprod();
            initactTab();
            initgoTab();
            actions.insert(make_pair("binop", makebinop)); 
            actions.insert(make_pair("unary", makeunary));
            actions.insert(make_pair("num", [](auto& a) { a[0]->attr.type = EXPR_NODE; a[0]->attr.expr = NUM_EXPR; return a[0]; }));
            actions.insert(make_pair("id", [](auto& a) { a[0]->attr.type = EXPR_NODE; a[0]->attr.expr = ID_EXPR; return a[0]; }));
            actions.insert(make_pair("string", [](auto& a) { a[0]->attr.type = EXPR_NODE; a[0]->attr.expr = STRING_EXPR; return a[0]; }));
            actions.insert(make_pair("mkexprstmt", [](auto& a) { return a[0]; }));
            actions.insert(make_pair("pass", [](auto& a) { return a[1]; }));
            actions.insert(make_pair("mklist",  makelist));
            actions.insert(make_pair("mkprint", makeprint)); 
            actions.insert(make_pair("mkif", makeIf)); 
            actions.insert(make_pair("mkblock", makeBlock)); 
            actions.insert(make_pair("mkfunc", makeFunc)); 
            actions.insert(make_pair("mkcall", makeCall)); 
            actions.insert(make_pair("mklet", makeLet));
            actions.insert(make_pair("mkret", makeRet)); 
            actions.insert(make_pair("mklistcon", makeListConstructor));
            actions.insert(make_pair("mksubscript",makeSubScript));
            actions.insert(make_pair("mkwhile", makeWhile)); 
            actions.insert(make_pair("mklambda", makeLambda));
            actions.insert(make_pair("mkstruct", makeStruct));
        }
        void doShift(int next) {
            cout<<"SHIFT"<<endl;
            st.push(next);
            semStack.push(new AST(current()));
            advance();
        }
        void doReduce(Production& X) {
            cout<<"REDUCE"<<endl;
            vector<AST*> tmp;
            for (int i = 0; i < X.rhs.size(); i++) {
                st.pop();
                tmp.push_back(semStack.top());
                semStack.pop();
            }
            reverse(tmp.begin(), tmp.end());
            if (X.action.empty() == false) {
                cout<<"And do: "<<X.action<<endl;
                semStack.push(actions[X.action.substr(1)](tmp));
                preorder(semStack.top(), 1);
            } else {
                for (auto m : tmp) {
                    if (m != nullptr) {
                        semStack.push(m);
                    }
                }
            }
            st.push(stoi(goTab[st.top()][X.lhs]));
        }
        void printCurrent(int state_num, Token& T) {
            //cout<<S.key()<<endl;
            cout<<"[ state: "<<state_num<<"][ token: "<<tokenStr[T.getSymbol()]<<"]"<<actTab[state_num][T.getString()]<<endl<<"Action: ";
        }
        bool checkAccept(int state_num, Token& T) {
            if (T.getSymbol() == TK_EOI && actTab[state_num]["$"] == "accept") {
                cout<<"ACCEPT"<<endl;
                return true;
            }
            return false;
        }
        AST* parse(vector<Token>& tok) {
            tokens = tok;
            tpos = 0;
            st.push(0);
            for (;;) {
                Token curr_token = current();
                int curr_state = st.top();
                printCurrent(curr_state, curr_token);
                if (checkAccept(curr_state, curr_token))
                    return semStack.top();
                if (actTab[curr_state].find(tokenStr[curr_token.getSymbol()]) == actTab[curr_state].end()) {
                    cout<<"Hmm, no actions on '"<<tokenStr[curr_token.getSymbol()]<<"'?"<<endl;
                    for (auto m : actTab[curr_state]) {
                        cout<<m.first<<": "<<m.second<<endl;
                    }
                    return nullptr;
                }
                string act = actTab[curr_state][tokenStr[curr_token.getSymbol()]];
                if (act == "accept")
                    return semStack.top();
                int next = stoi(act.substr(1));
                switch (act[0]) {
                    case 's': {
                        doShift(next);
                    } break;
                    case 'r': {
                        Production p = prod[next];
                        doReduce(p);
                    } break;
                    default:
                        cout<<"Syntax Error: "<<tokenStr[curr_token.getSymbol()]<<endl;
                        return nullptr;
                }
            }
            return nullptr;
        }
    };

#endif