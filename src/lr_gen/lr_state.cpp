#include "lr_state.hpp"

LRState::LRState() : state_num(-1), re_calc_key(true) {

}

string LRState::key() const {
    if (re_calc_key) {
        set<string> strs;
        set<string> item_keys;
        for (const auto& item : items) {
            strs.insert(item.toString());
            item_keys.insert(to_string(item.getProduction().pid) + "@" + to_string(item.getDotPosition()));
        }
        string result;
        string cresult;
        auto rit = strs.begin();
        auto cit  = item_keys.begin();
        while (rit != strs.end() && cit != item_keys.end()) {
            result += *rit + "\n";
            cresult += *cit + "|";
            rit++;
            cit++;
        }
        cached_key = result;
        cached_core = cresult;
        re_calc_key = false;
    }
    return cached_key;
}

string LRState::coreKey() const {
    if (re_calc_key) key();
    return cached_core;
}

bool LRState::mergeLookaheadsFrom(const LRState& other) {
    bool changed = false;
    for (const LRItem& incomingItem : other.getItems()) {
        auto existing = find_if(items.begin(), items.end(), [&](const LRItem& lri) { return lri.kernel() == incomingItem.kernel(); });
        if (existing != items.end()) {
            LRItem existingItem = *existing;
            items.erase(existing);
            size_t oldSize = existingItem.lookaheads().size();
            existingItem.lookaheads().insert(incomingItem.lookaheads().begin(), incomingItem.lookaheads().end());
            if (existingItem.lookaheads().size() > oldSize) {
                changed = true;
                existingItem.rehash();
            }
            items.insert(existingItem);
        }
    }
    return changed;
}

int LRState::getStateNum() {
    return state_num;
}

void LRState::setStateNum(int sn) {
    state_num = sn;
}

bool LRState::hasItem(const LRItem& item) {
    return items.find(item) != items.end();
} 

void LRState::addItem(LRItem item) {
    items.insert(item);
    re_calc_key = true;
}

unordered_set<LRItem> LRState::getItems() const {
    return items;
}
unordered_set<LRItem>& LRState::mutableItems() {
    return items;
}

bool LRState::operator==(const LRState& other) const {
    return key() == other.key();
}
bool LRState::operator!=(const LRState& other) const {
    return !(*this == other);
}