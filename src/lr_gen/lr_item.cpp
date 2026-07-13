#include "lr_item.hpp"

namespace std {
        std::size_t hash<LRItem>::operator()(const LRItem& item) const {
            size_t h1 = hash<int>()(item.production.pid);
            size_t h2 = hash<int>()(item.dotPosition);
            return h1 ^ (h2 << 1);
        }
}

LRItem::LRItem(Production p, int dp) : production(p), dotPosition(dp) { }

LRItem::LRItem(const LRItem& lri) {
    production = lri.production;
    dotPosition = lri.dotPosition;
}

LRItem& LRItem::operator=(const LRItem& lri) {
    if (this != &lri) {
            production = lri.production;
            dotPosition = lri.dotPosition;
    }
    return *this;
}

Symbol LRItem::symbolAfterDot() {
    if (complete())
        return "<fin>";
    return production.rhs.at(dotPosition);
}

bool LRItem::complete() const {
    return dotPosition >= production.rhs.size();
}

LRItem LRItem::advance() {
    return LRItem(production, dotPosition + 1);
}

bool LRItem::operator==(const LRItem& other) const {
    return production == other.production && dotPosition == other.dotPosition;
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
    return result;

}

void LRItem::print() {
    cout<<toString()<<endl;
}