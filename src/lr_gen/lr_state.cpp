#include "lr_state.hpp"

LRState::LRState() : state_num(-1), re_calc_key(true) {

}

string LRState::key() const {
    if (re_calc_key) {
        vector<string> strs;
        set<string> item_keys;
        for (const auto& item : items) {
            strs.push_back(item.toString());
            item_keys.insert(to_string(item.getProduction().pid) + "@" + to_string(item.getDotPosition()));
        }
        sort(strs.begin(), strs.end());
        string result;
        for (const auto& s : strs) {
            result += s + "\n";
        }
        cached_key = result;
        string cresult;
        for (const auto& s : item_keys) {
            cresult += s + "|";
        }
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
        // Find the matching item core in our current state
        for (auto it = items.begin(); it != items.end(); ++it) {
            // Safe cast: modifying lookaheads does not change the hash or equality of LRItem
            if (incomingItem.getDotPosition() == (*it).getDotPosition() && incomingItem.getProduction() == (*it).getProduction()) {
                LRItem& existingItem = const_cast<LRItem&>(*it);
                size_t oldSize = existingItem.lookaheads().size();
                existingItem.lookaheads().insert(incomingItem.lookaheads().begin(), incomingItem.lookaheads().end());
                if (existingItem.lookaheads().size() > oldSize) {
                    changed = true;
                }
                break;
            }
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