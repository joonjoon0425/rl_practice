#include "core/qtable.hpp"
#include "core/common.hpp"
#include <cassert>
#include <limits>

QTables::QTables(int state_size, int action_size, float init, int table_num)
: table_num_(table_num),
    state_size_(state_size),
    action_size_(action_size),
    Q_table_(table_num * state_size * action_size, init) {}

float& QTables::operator()(state_t state, action_t action, int table) {
    assert(-1 <= table && table < table_num_ && "out of index");

    if (table != -1) return Q_table_[table * (state_size_ * action_size_) + state * action_size_ + action];
    else return Q_table_[state * action_size_ + action]; // implement qtable estimator here! For now, we use single one.
}

float QTables::operator()(state_t state, action_t action, int table) const {
    assert(-1 <= table && table < table_num_ && "out of index");

    if (table != -1) return Q_table_[table * (state_size_ * action_size_) + state * action_size_ + action];
    else return Q_table_[state * action_size_ + action]; // implement qtable estimator here! For now, we use single one.
}

action_t QTables::greedy_action(const state_t& state, const action_mask_t& possible_actions) const {
    auto max_actions = greedy_actions(state, possible_actions);
    action_mask_internal_t mask = max_actions.to_ullong();
    
    assert(mask > 0 && "no actions");
    int size = max_actions.count();

    int rand = std::rand() % size;

    for (int i = 0; i < rand; i++) mask &= (mask - 1);

    assert(mask != 0 && "mask became zero unexpectedly");
    return __builtin_ctzll(mask);
}

action_mask_t QTables::greedy_actions(const state_t& state, const action_mask_t& possible_actions) const {
    action_mask_t max_actions;
    action_mask_internal_t mask = possible_actions.to_ullong();
    float max_ = -std::numeric_limits<float>::infinity();
    assert(mask > 0 && "no possible actions");
        
    while (mask > 0) {
        int action = __builtin_ctzll(mask);
        float q = (*this)(state, action);

        if (max_ < q) {
            max_actions.reset();
            max_actions |= 1ULL << action;
            max_ = q;
        } else if (max_ - q < 1e-7f) {
            max_actions |= 1ULL << action;
        }
        mask &= (mask - 1);
    }

    return max_actions;
}

float QTables::max(const state_t& state, const action_mask_t& possible_actions) const {
    action_mask_internal_t mask = possible_actions.to_ullong();
    assert(mask > 0 && "no possible actions");
    float max_ = -std::numeric_limits<float>::infinity();

    while (mask > 0) {
        int action = __builtin_ctzll(mask);
        float q = (*this)(state, action);
        max_ = q < max_ ? max_ : q;
        mask &= (mask - 1);
    }

    return max_;
}