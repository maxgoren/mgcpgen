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
        mutable vector<LRItem> items;
        mutable unordered_map<string, int> bykern;
        mutable string cached_key;
        mutable string cached_core;
        mutable bool re_calc_key;
    public:
        LRState();
        string& key() const;
        string& coreKey() const;
        void update_cache() const;
        bool mergeLookaheadsFrom(const LRState& other);
        int getStateNum() const;
        void setStateNum(int sn);
        bool hasItem(const LRItem& item);
        void addItem(LRItem item);
        vector<LRItem>& getItems() const;
        bool operator==(const LRState& other) const;
        bool operator!=(const LRState& other) const;
};

#endif