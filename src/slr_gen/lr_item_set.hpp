#ifndef lr_item_set_hpp
#define lr_item_set_hpp
#include <queue>
#include <unordered_set>
#include "../../src/cfg.hpp"
using namespace std;

struct LRItem {
    Production production;
    int dotPosition;
    LRItem(Production p, int dp) : production(p), dotPosition(dp) { }
    LRItem(const LRItem& lri) {
        production = lri.production;
        dotPosition = lri.dotPosition;
    }
    LRItem& operator=(const LRItem& lri) {
        if (this != &lri) {
             production = lri.production;
             dotPosition = lri.dotPosition;
        }
        return *this;
    }
    Symbol symbolAfterDot() {
        if (complete())
            return "<fin>";
        //cout<<production.toString()<<": ";
        //cout<<"DP: "<<dotPosition<<", size: "<< production.rhs.size()<<endl;
        return production.rhs.at(dotPosition);
    }
    bool complete() const {
        return dotPosition >= production.rhs.size();
    }
    LRItem advance() {
        return LRItem(production, dotPosition + 1);
    }
    bool operator==(const LRItem& other) const {
        return production == other.production && dotPosition == other.dotPosition;
    }
    string toString() const {
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
    void print() {
        cout<<toString()<<endl;
    }
};

namespace std {
    template <> struct hash<LRItem> {
        std::size_t operator()(const LRItem& item) const {
            size_t h1 = hash<int>()(item.production.pid);
            size_t h2 = hash<int>()(item.dotPosition);
            return h1 ^ (h2 << 1);
        }
    };
}

struct LRState {
    int state_num;
    unordered_set<LRItem> items;
    string key() const {
        vector<string> strs;
        for (const auto& item : items) {
            strs.push_back(item.toString());
        }
        sort(strs.begin(), strs.end());
        string result;
        for (const auto& s : strs) {
            result += s + "\n";
        }
        return result;
    }
    bool operator==(const LRState& other) const {
        return key() == other.key();
    }
    bool operator!=(const LRState& other) const {
        return !(*this == other);
    }
};


#endif