#include "lr_state.hpp"
LRState::LRState() : state_num(-1), re_calc_key(true) {

}

string& LRState::key() const {
    if (re_calc_key) update_cache();
    return cached_key;
}

string& LRState::coreKey() const {
    if (re_calc_key) update_cache();
    return cached_core;
}

void LRState::update_cache() const {
    if (re_calc_key == false)
        return;
    set<string> strs;
    set<string> item_keys;
    for (const auto& item : items) {
        strs.insert(item.toString());
        item_keys.insert(item.kernel());
    }
    cached_key.clear();
    cached_core.clear();
    for (auto rit : strs) {
        cached_key += rit + "\n";
    }
    for (auto cit : item_keys) {
        cached_core += cit + "|";
    }
    re_calc_key = false;
}

bool LRState::mergeLookaheadsFrom(const LRState& other) {
    bool changed = false;
    for (const LRItem& incomingItem : other.items) {
        auto it = bykern.find(incomingItem.kernel());
        if (it != bykern.end()) {
            int idx = it->second;
            auto& current_la = items[idx].lookaheads();
            const auto& incoming = incomingItem.lookaheads();
            int os = current_la.count();
            current_la |= incoming;
            if (current_la.count() > os) {
                changed = true;
                items[idx].rehash();
                re_calc_key = true;
            }
        }
    }
    return changed;
}

int LRState::getStateNum() const {
    return state_num;
}

void LRState::setStateNum(int sn) {
    state_num = sn;
}

bool LRState::hasItem(const LRItem& item) {
    return find(items.begin(), items.end(), item) != items.end();
} 

void LRState::addItem(LRItem item) {
    items.push_back(item);
    bykern[item.kernel()] = items.size() - 1;
    re_calc_key = true;
}

vector<LRItem>& LRState::getItems() const {
    return items;
}

bool LRState::operator==(const LRState& other) const {
    return key() == other.key();
}
bool LRState::operator!=(const LRState& other) const {
    return !(*this == other);
}