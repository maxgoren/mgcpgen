#ifndef lr_state_hpp
#define lr_state_hpp
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include "lr_item.hpp"
using namespace std;

class LRState {
    private:
        int state_num;
        mutable unordered_set<LRItem> items;
        mutable string cached_key;
        mutable string cached_core;
        mutable bool re_calc_key;
    public:
        LRState();
        string key() const;
        string coreKey() const;
        bool mergeLookaheadsFrom(const LRState& other);
        int getStateNum();
        void setStateNum(int sn);
        bool hasItem(const LRItem& item);
        void addItem(LRItem item);
        unordered_set<LRItem> getItems() const;
        unordered_set<LRItem>& mutableItems();
        bool operator==(const LRState& other) const;
        bool operator!=(const LRState& other) const;
};

#endif