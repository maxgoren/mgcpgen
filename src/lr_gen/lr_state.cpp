#include "lr_state.hpp"

LRState::LRState() : state_num(-1), re_calc_key(true) {

}

string LRState::key() const {
    if (re_calc_key) {
        vector<string> strs;
        for (const auto& item : items) {
            strs.push_back(item.toString());
        }
        sort(strs.begin(), strs.end());
        string result;
        for (const auto& s : strs) {
            result += s + "\n";
        }
        cached_key = result;
        re_calc_key = false;
    }
    return cached_key;
}

string LRState::coreKey() const {
    set<string> item_keys;
    for (const LRItem& item : items) {
        item_keys.insert(to_string(item.getProduction().pid) + "@" + to_string(item.getDotPosition()));
    }    
    string result;
    for (const string& ik : item_keys) {
        result += ik + "|";
    }
    return result;
}

bool LRState::mergeLookaheadsFrom(const LRState& other) {
    bool changed = false;
    for (const LRItem& incomingItem : other.getItems()) {
        // Find the matching item core in our current state
        auto it = items.begin();
        while (it != items.end()) {
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
            it++;
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