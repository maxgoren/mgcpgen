#include "lr_state.hpp"

LRState::LRState() : state_num(-1) {

}

string LRState::key() const {
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
}

unordered_set<LRItem> LRState::getItems() const {
    return items;
}

bool LRState::operator==(const LRState& other) const {
    return key() == other.key();
}
bool LRState::operator!=(const LRState& other) const {
    return !(*this == other);
}