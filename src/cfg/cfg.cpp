#include "cfg.hpp"
extern "C" {
    #include <mgc/mgclex.h>
}

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
    startSym = -1;
}
bool Grammar::isNonTerminal(Symbol s) {
    return nonterminals.find(s) != nonterminals.end();
}
bool Grammar::isTerminal(Symbol s) {
    return terminals.find(s) != terminals.end();
}
bool Grammar::isNullable(Symbol nt) {
    if (nullable.find(nt) != nullable.end())
        return nullable[nt];
    return false;
}

void Grammar::parseProduction(vector<string>& parts, int rulenum) {
    int prodid = symMap.get(parts[0]); 
    nonterminals.insert(prodid);
    ProductionSet ps = productions[prodid];
    SymbolString ss;
    string actmaybe;
    Symbol precOverSym = -1;
    for (int i = 2; i < parts.size(); i++) {
        if (parts[i].empty()) {
            continue;
        } else if (parts[i] == "prec_override") {
            precOverSym = i+1 < parts.size() ? symMap.get(parts[i+1]):-1;
            cout<<"Precedence override directive: "<<precOverSym<<endl;
            break;
        }
        Symbol s = symMap.get(parts[i]);
        if (terminals.find(s) != terminals.end()) {
            ss.push_back(s);
        } if (parts[i][0] != '@' && terminals.find(s) == terminals.end()) {
            nonterminals.insert(s);
            ss.push_back(s);
        } else if (parts[i][0] == '@') {
            actmaybe = symMap.name(s);
            actionMap.insert({parts[i].substr(1), parts[i].substr(1)});
        }
    }
    if (parts[2][0] == '#') ss.clear();
    ps.push_back(Production(rulenum, prodid, ss, actmaybe, precOverSym));
    productions[prodid] = ps;
    prodById[ps.back().pid] = ps.back();
    if (startSym == -1) {
        startSym = symMap.get(parts[0]);
    }
}

vector<string> readTokenList(string filename) {
    ifstream ifile(filename);
    vector<string> symbols;
    if (ifile.is_open()) {
        string buffer;
        while (getline(ifile, buffer)) {
            if (buffer.back() == '\n' || buffer.back() == '\r') {
                buffer.pop_back();
            }
            symbols.push_back(buffer);
            buffer.clear();
        }
    } else {
        cout<<"Couldnt open "<<filename<<endl;
    }
    return symbols;
}

void Grammar::populateTerminalList(string filename, bool generate) {
    if (generate) {
        char outname[17] = "lexer_matrix.h";
        TerminalList* tl = generateLexer(filename.data(), outname, true, true, 0);
        for (int i = 0; i < tl->num_symbols; i++) {
            int id = symMap.get(tl->symbols[i]);
            terminals.insert(id);
            free(tl->symbols[i]);
        }
        free(tl);
        terminals.insert(symMap.get(GOAL));
        terminals.insert(symMap.get(EPS));
        terminals.insert(symMap.get(ACTSYM));
    } else {
        vector<string> syms = readTokenList(filename);
        for (auto symbol : syms) {
            terminals.insert(symMap.get(symbol));
        }
    }
}

bool Grammar::readGrammarFile(string filename, bool and_lexer) {
    string buff;
    ifstream infile(filename, ios::in);
    if (!infile.is_open()) {
        cout<<"Error: Couldn't open '"<<filename<<"' - unknown file."<<endl;
        return false;
    }
    int rulenum = 1;
    while (infile.good()) {
        getline(infile, buff);
        if (buff.size() && buff.back() == '\r')
            buff.pop_back();
        if (buff.empty() || buff == "\n" || (buff[0] == '/' && buff[1] == '/'))
            continue;
        vector<string> parts = split(buff, ' ');
        if (parts[0] == "terminal_list") {
            populateTerminalList(parts[1], and_lexer);
        } else if (parts[0] == "type_returned") {
            returnType = parts[1];
        } else if (parts[0] == "set_token_prec") {
            Symbol symbol = symMap.get(parts[1]);
            precedenceMap.insert(make_pair(symbol,OpPrec(symbol,stoi(parts[2]), (parts.size() == 4 ? parts[3]:"none"))));
        } else {
            parseProduction(parts, rulenum);
            rulenum++;
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