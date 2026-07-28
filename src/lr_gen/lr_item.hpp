#ifndef lr_item_set_hpp
#define lr_item_set_hpp
#include <queue>
#include <unordered_set>
#include "../../src/cfg/cfg.hpp"
using namespace std;

class LRItem {
    private:
        Production production;
        int dotPosition;
        unordered_set<Symbol> la_set;
    public:
        LRItem(Production p, int dp) ;
        LRItem(const LRItem& lri);
        int getDotPosition() const;
        Production getProduction() const;
        Symbol symbolAfterDot();
        SymbolString betaSymbols();
        unordered_set<Symbol> lookaheads() const;
        unordered_set<Symbol>& lookaheads();
        bool complete() const;
        LRItem advance();
        string toString() const;
        void print();
        LRItem& operator=(const LRItem& lri);
        bool operator==(const LRItem& other) const;
};

namespace std {
    template <> struct hash<LRItem> {
        std::size_t operator()(const LRItem&) const;
    };
}

#endif