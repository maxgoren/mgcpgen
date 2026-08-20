#include "cfg.hpp"

vector<string> split(string input, char delim) {
    vector<string> result;
    string buffer;
    for (char c : input) {
        if (c != delim) {
            buffer.push_back(c);
        } else {
            result.push_back(buffer);
            buffer.clear();
        }
    }
    result.push_back(buffer);
    return result;
}

Grammar::Grammar() {
    startSym = "";
}
bool Grammar::isNonTerminal(Symbol s) {
    return nonterminals.find(s) != nonterminals.end();
}
bool Grammar::isTerminal(Symbol s) {
    return terminals.find(s) != terminals.end();
}
bool Grammar::isNullable(Symbol nt) {
    if (derivesLambda.find(nt) != derivesLambda.end())
        return derivesLambda[nt];
    return false;
}
bool Grammar::readGrammarFile(string filename) {
    string buff;
    ifstream infile(filename, ios::in);
    if (!infile.is_open()) {
        cout<<"Error: Couldn't open '"<<filename<<"' - unknown file."<<endl;
        return false;
    }
    int rulenum = 1;
    string lastrule = "";
    while (infile.good()) {
        getline(infile, buff);
        if (buff.size() && buff.back() == '\r')
            buff.pop_back();
        if (buff.empty())
            continue;
        vector<string> parts = split(buff, ' ');
        if (parts[0][0] == '/' && parts[0][1] == '/') 
            continue;
        if (parts[0] == "set_token_prec") {
            string symbol = parts[1];
            int prec = stoi(parts[2]);
            string assoc = parts.size() == 4 ? parts[3]:"none";
            precedenceMap.insert(make_pair(symbol,OpPrec(symbol, prec, assoc)));
        } else {
            nonterminals.insert(parts[0]);
            ProductionSet ps = productions[parts[0]];
            SymbolString ss;
            terminals.insert(GOAL);
            terminals.insert(EPS);
            string actmaybe;
            Symbol precOverSym;
            for (int i = 2; i < parts.size(); i++) {
                string s = parts[i];
                if (s.empty())
                    continue;
                if (s[0] == 'T' && s[1] == 'K' && s[2] == '_') {
                    terminals.insert(s);
                    ss.push_back(s);
                } else if (s == "prec_override") {
                    cout<<"Got an override directive: ";
                    precOverSym = parts[i+1];
                    cout<<precOverSym<<endl;
                    break;
                } else if (s[0] != '@') {
                    nonterminals.insert(s);
                    ss.push_back(s);
                } else if (s[0] == '@') {
                    actmaybe = s;
                    actionMap.insert({s.substr(1), s.substr(1)});
                }
            }
            if (ss[0] == "#") ss.clear();
            ps.push_back(Production(rulenum++, parts[0], ss, actmaybe));
            if (!precOverSym.empty()) {
                ps.back().precOverride = precOverSym;
            }
            productions[parts[0]] = ps;
            prodById[ps.back().pid] = ps.back();
            lastrule = parts[0];
            if (startSym == "") {
                startSym = lastrule;
            }
        }
    }
    return true;
}

ProductionSet::ProductionSet(vector<Production> rhs) {
    for (Production ss : rhs) {
        push_back(ss);
    }
}

ProductionSet::ProductionSet() {

}