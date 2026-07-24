#ifndef lr_state_hpp
#define lr_state_hpp
#include <iostream>
#include <unordered_set>
#include "lr_item.hpp"
using namespace std;

class LRState {
    private:
    int state_num;
    unordered_set<LRItem> items;
    mutable string cached_key;
    mutable bool re_calc_key;
    public:
    LRState();
    string key() const;
    int getStateNum();
    void setStateNum(int sn);
    bool hasItem(const LRItem& item);
    void addItem(LRItem item);
    unordered_set<LRItem> getItems() const;
    bool operator==(const LRState& other) const;
    bool operator!=(const LRState& other) const;
};

#endif