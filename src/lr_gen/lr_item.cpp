#include "lr_item.hpp"

namespace std {
        std::size_t hash<LRItem>::operator()(const LRItem& item) const {
                size_t h1 = hash<int>()(item.getProduction().pid);
                size_t h2 = hash<int>()(item.getDotPosition());
                return h1 ^ (h2 << 1);
        }
}

LRItem::LRItem(Production p, int dp) : production(p), dotPosition(dp) {  }
LRItem::LRItem() : dotPosition(-1) { }
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

string LRItem::kernel() const {
    return to_string(production.pid) + "@" + to_string(dotPosition);
}

LRItem LRItem::advance() {
    return LRItem(production, dotPosition + 1);
}

bool LRItem::operator==(const LRItem& other) const {
    return production == other.production && dotPosition == other.dotPosition && la_set == other.la_set;
}

string LRItem::toString() const {
    int i = 0;
    string result = kernel();
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