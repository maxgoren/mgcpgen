#include "lr_item.hpp"

namespace std {
        std::size_t hash<LRItem>::operator()(const LRItem& item) const {
            size_t h1 = hash<int>()(item.getProduction().pid);
            size_t h2 = hash<int>()(item.getDotPosition());
            size_t hash_value = h1 ^ (h2 << 1);
            std::vector<Symbol> sorted_la(item.lookaheads().begin(), item.lookaheads().end());
            std::sort(sorted_la.begin(), sorted_la.end());
            for (const Symbol& la : sorted_la) {
                hash_value ^= hash<string>()(la) + 0x9e3779b9 + (hash_value << 6) + (hash_value >> 2);
            }
            return hash_value;
        }
}

LRItem::LRItem(Production p, int dp) : production(p), dotPosition(dp) { }

LRItem::LRItem(const LRItem& lri) {
    production = lri.production;
    dotPosition = lri.dotPosition;
    la_set = lri.la_set;
}

LRItem& LRItem::operator=(const LRItem& lri) {
    if (this != &lri) {
        production = lri.production;
        dotPosition = lri.dotPosition;
        la_set = lri.la_set;
    }
    return *this;
}

int LRItem::getDotPosition() const {
    return dotPosition;
}

Production LRItem::getProduction() const {
    return production;
}

Symbol LRItem::symbolAfterDot() {
    if (complete())
        return "<fin>";
    return production.rhs.at(dotPosition);
}

SymbolString LRItem::betaSymbols() {
    return production.rhs.subString(dotPosition + 1);
}
unordered_set<Symbol> LRItem::lookaheads() const {
    return la_set;
}
unordered_set<Symbol>& LRItem::lookaheads() {
    return la_set;
}

bool LRItem::complete() const {
    return dotPosition >= production.rhs.size();
}

LRItem LRItem::advance() {
    return LRItem(production, dotPosition + 1);
}

bool LRItem::operator==(const LRItem& other) const {
    return production == other.production && dotPosition == other.dotPosition && la_set == other.la_set;
}

string LRItem::toString() const {
    int i = 0;
    string result;
    result += production.lhs;
    result += " ::= ";
    for (i = 0; i < production.rhs.size(); i++) {
        if (i == dotPosition)
            result += ". ";
        result += production.rhs[i];
        result += " ";
    }
    if (dotPosition == production.rhs.size()) 
        result += ".";
    vector<Symbol> sorted_la(la_set.begin(), la_set.end());
    sort(sorted_la.begin(), sorted_la.end());
    result += " { ";
    for (auto la : sorted_la) {
        result += la + " ";
    }
    result += "}";
    return result;

}

void LRItem::print() {
    cout<<toString()<<endl;
}