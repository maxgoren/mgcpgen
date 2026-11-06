#ifndef lr_item_set_hpp
#define lr_item_set_hpp
#include <queue>
#include <bitset>
#include "../../src/cfg/cfg.hpp"
using namespace std;

constexpr int MAX_LA_BITS = 512;

class LRItem {
    private:
        Production production;
        int dotPosition;
        bitset<MAX_LA_BITS> la_set;
        size_t hash_val;
    public:
        LRItem();
        ~LRItem();
        LRItem(Production p, int dp);
        LRItem(const LRItem& lri);
        int getDotPosition() const;
        Production getProduction() const;
        Symbol symbolAfterDot();
        SymbolString betaSymbols();
        bitset<MAX_LA_BITS> lookaheads() const;
        bitset<MAX_LA_BITS>& lookaheads();
        bool complete() const;
        LRItem advance();
        int kernel() const;
        string toString() const;
        void rehash();
        size_t hashCode() const;
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