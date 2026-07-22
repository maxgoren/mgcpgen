#include "production.hpp"
#include <vector>
using namespace std;


SymbolString::SymbolString(vector<Symbol> ss) {
    for (auto m : ss) {
        this->push_back(m);
    }
}

SymbolString::SymbolString() {

}

SymbolString::SymbolString(const SymbolString& ss) {
    for (auto m : ss) {
        this->push_back(m);
    }
}

SymbolString SymbolString::subString(int startIndex) {
    SymbolString result;
    for (int i = startIndex; i < this->size(); i++)
        result.push_back((*this)[i]);
    return result;
}
SymbolString& SymbolString::operator=(const SymbolString& ss) {
    if (this != &ss) {
        for (auto m : ss) {
            this->push_back(m);
        }
    }
    return *this;
}
string SymbolString::toString() {
    string str = "";
    for (Symbol s : *this) {
        str += s + " ";
    }
    return str;
}
bool SymbolString::operator==(const SymbolString& ss) {
    auto oit = ss.begin();
    auto it = this->begin();
    while (oit != ss.end() && it != this->end()) {
        if (*oit != *it)
            return false;
    }
    return (oit == ss.end() && it == this->end());
}
bool SymbolString::operator!=(const SymbolString& ss) {
    return !(*this == ss);
}

Production::Production(int id, Symbol l, SymbolString r, string a) : pid(id), lhs(l), rhs(r), action(a) { }
Production::Production(int id, Symbol l, SymbolString r) : pid(id), lhs(l), rhs(r) { }
Production::Production() { }
string Production::toString() {
    return lhs + " ::= " + rhs.toString(); 
}
bool Production::operator==(const Production& op) const {
    return lhs == op.lhs && rhs == op.rhs;
}
bool Production::operator!=(const Production& op) const {
    return !(*this==op);
}
bool Production::operator<(const Production& op) const {
    return this->lhs < op.lhs;
}