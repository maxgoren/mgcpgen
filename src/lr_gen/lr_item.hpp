#ifndef lr_item_set_hpp
#define lr_item_set_hpp
#include <queue>
#include <unordered_set>
#include "../../src/cfg.hpp"
using namespace std;

struct LRItem {
    Production production;
    int dotPosition;
    LRItem(Production p, int dp) ;
    LRItem(const LRItem& lri);
    LRItem& operator=(const LRItem& lri);
    Symbol symbolAfterDot();
    bool complete() const;
    LRItem advance();
    bool operator==(const LRItem& other) const;
    string toString() const;
    void print();
};
namespace std {
    template <> struct hash<LRItem> {
        std::size_t operator()(const LRItem&) const;
    };
}

#endif