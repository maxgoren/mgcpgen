#ifndef lr_state_hpp
#define lr_state_hpp
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include "lr_item.hpp"
using namespace std;
/*
template <class T, class K>
class BiMapSet {
    private:
        vector<T> data;
        unordered_map<T, int> by_type;
        unordered_map<K, int> by_key;
    public:
        BiMapSet() {

        }
        void insert(T item, K key) {
            int idx = data.size();
            data.push_back(item);
            by_type.insert({item, idx});
            by_key.insert({key, idx});
        }
        bool hasItem(T item) {
            return by_type.count(item);
        }
        bool hasItem(K key) {
            return by_key.count(key);
        }
        vector<T>::iterator begin() {
            return data.begin();
        }
        vector<T>::iterator end() {
            return data.end();
        }
        vector<T>::iterator begin() const {
            return data.begin();
        }
        vector<T>::iterator end() const {
            return data.end();
        }
};
*/
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