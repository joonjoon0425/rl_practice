#include "base/policy/greedyPolicy.hpp"
#include <cstdlib>
#include <limits>

greedyPolicy::greedyPolicy(){}

action_t greedyPolicy::get_action(const QValueSource& Q_table_, const state_t& state, const action_mask_t& possible_actions) const {
    return Q_table_.greedy_action(state, possible_actions);
}

float greedyPolicy::get_prob(const QValueSource& Q_table_, const state_t& state, const action_t& action, const action_mask_t& possible_actions) const {
    auto max_actions = Q_table_.greedy_actions(state, possible_actions);
    return float(max_actions[action]) / max_actions.count();
};