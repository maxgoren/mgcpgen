#ifndef slr_parser_hpp
#define slr_parser_hpp
#include <iostream>
#include <functional>
#include <stack>
#include "actions.hpp"
#include "mgc_slr_gen.out.hpp"

using namespace std;

class SLRParser {
    private:
        stack<AST*> semStack;
        stack<int> st;
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
        SLRParser() {
            initprod();
            initactTab();
            initgoTab();
            actions.insert(make_pair("binop", [](auto& a) { return makebinop(a); }));
            actions.insert(make_pair("unary", [](auto& a) { return makeunary(a); }));
            actions.insert(make_pair("num", [](auto& a) { a[0]->attr.type = EXPR_NODE; a[0]->attr.expr = NUM_EXPR; return a[0]; }));
            actions.insert(make_pair("id", [](auto& a) { a[0]->attr.type = EXPR_NODE; a[0]->attr.expr = ID_EXPR; return a[0]; }));
            actions.insert(make_pair("string", [](auto& a) { a[0]->attr.type = EXPR_NODE; a[0]->attr.expr = STRING_EXPR; return a[0]; }));
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
            actions.insert(make_pair("mklistcon", [](auto& a) { return makeListConstructor(a); }));
            actions.insert(make_pair("mksubscript", [](auto& a) { return makeSubScript(a); }));
            actions.insert(make_pair("mkwhile", [](auto& a) { return makeWhile(a); }));
            actions.insert(make_pair("mklambda", [](auto& a) { return makeLambda(a); }));
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
                //printCurrent(curr_state, curr_token);
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