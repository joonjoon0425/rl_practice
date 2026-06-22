#include "core/QValueSource.hpp"
#include "core/common.hpp"
#include <cassert>
#include <limits>

QValueSource::QValueSource(int state_size, int action_size, float init, int table_size, std::unique_ptr<QValueEstimator> estimator)
: table_size_(table_size),
    state_size_(state_size),
    action_size_(action_size),
    Q_table_(table_size * state_size * action_size, init),
    estimator_(std::move(estimator)) {}

float& QValueSource::operator()(state_t state, action_t action, int table) {
    assert(0 <= table && table < table_size_ && "out of index");

    return Q_table_[table * (state_size_ * action_size_) + state * action_size_ + action];
}

QValueSource::Proxy QValueSource::operator()(state_t state, action_t action) {
    return Proxy {*this, state, action};
}

float QValueSource::operator()(state_t state, action_t action, int table) const {
    assert(-1 <= table && table < table_size_ && "out of index");

    if (table != -1) return Q_table_[table * (state_size_ * action_size_) + state * action_size_ + action];
    else if (estimator_) return estimator_->estimate(Q_table_, table_size_, state_size_, action_size_, state, action);
    else return Q_table_[state * action_size_ + action];
}

action_t QValueSource::greedy_action(const state_t& state, const action_mask_t& possible_actions, int table) const {
    auto max_actions = greedy_actions(state, possible_actions, table);
    action_mask_internal_t mask = max_actions.to_ullong();
    
    assert(mask > 0 && "no actions");
    int size = max_actions.count();

    int rand = std::rand() % size;

    for (int i = 0; i < rand; i++) mask &= (mask - 1);

    assert(mask != 0 && "mask became zero unexpectedly");
    return __builtin_ctzll(mask);
}

action_mask_t QValueSource::greedy_actions(const state_t& state, const action_mask_t& possible_actions, int table) const {
    action_mask_t max_actions;
    action_mask_internal_t mask = possible_actions.to_ullong();
    float max_ = -std::numeric_limits<float>::infinity();
    assert(mask > 0 && "no possible actions");
        
    while (mask > 0) {
        int action = __builtin_ctzll(mask);
        float q = (*this)(state, action, table);

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

float QValueSource::max(const state_t& state, const action_mask_t& possible_actions, int table) const {
    action_mask_internal_t mask = possible_actions.to_ullong();
    assert(mask > 0 && "no possible actions");
    float max_ = -std::numeric_limits<float>::infinity();

    while (mask > 0) {
        int action = __builtin_ctzll(mask);
        float q = (*this)(state, action, table);
        max_ = q < max_ ? max_ : q;
        mask &= (mask - 1);
    }

    return max_;
}