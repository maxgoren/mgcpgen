#include "lr_item.hpp"

namespace std {
        std::size_t hash<LRItem>::operator()(const LRItem& item) const {
            return item.hashCode();
        }
}
LRItem::~LRItem() { }
LRItem::LRItem(Production p, int dp) : production(p), dotPosition(dp) { rehash(); }
LRItem::LRItem() : dotPosition(-1) { }
LRItem::LRItem(const LRItem& lri) {
    production = lri.production;
    dotPosition = lri.dotPosition;
    la_set = lri.la_set;
    hash_val = lri.hash_val;
    rehash();
}

LRItem& LRItem::operator=(const LRItem& lri) {
    if (this != &lri) {
        production = lri.production;
        dotPosition = lri.dotPosition;
        la_set = lri.la_set;
        hash_val = lri.hash_val;
        rehash();
    }
    return *this;
}

size_t LRItem::hashCode() const {
    return hash_val;
}

void LRItem::rehash() {
    size_t h1 = hash<int>()(production.pid);
    size_t h2 = hash<int>()(dotPosition);
    hash_val = h1 ^ (h2 << 1);
    for (int i = 0; i < 256; i++)
        if (la_set.test(i)) hash_val += i;
}


int LRItem::getDotPosition() const {
    return dotPosition;
}

Production LRItem::getProduction() const {
    return production;
}

Symbol LRItem::symbolAfterDot() {
    if (complete())
        return -1;
    return production.rhs.at(dotPosition);
}

SymbolString LRItem::betaSymbols() {
    return production.rhs.subString(dotPosition + 1);
}
bitset<256> LRItem::lookaheads() const {
    return la_set;
}
bitset<256>& LRItem::lookaheads() {
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
    result += " { " + la_set.to_string() + "}";
    return result;

}

void LRItem::print() {
    cout<<toString()<<endl;
}