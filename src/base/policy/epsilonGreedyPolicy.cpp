#include "base/policy/epsilonGreedyPolicy.hpp"
#include <cstdlib>
#include <limits>

epsilonGreedyPolicy::epsilonGreedyPolicy(float epsilon) : epsilon_(epsilon) {}

action_t epsilonGreedyPolicy::get_action(const QTables& Q_table_, const state_t& state, const action_mask_t& possible_actions) const {
    action_t ret_action;
    float rand_val = static_cast<float>(std::rand()) / RAND_MAX;

    if (rand_val < epsilon_) {
        //explore
        ret_action = random_action(state, possible_actions);
    } else {
        //exploit
        ret_action = greedy_action(Q_table_, state, possible_actions);
    }

    return ret_action;
}

action_t epsilonGreedyPolicy::greedy_action(const QTables& Q_table_, const state_t& state, const action_mask_t& possible_actions) const {
    return Q_table_.greedy_action(state, possible_actions);
}

float epsilonGreedyPolicy::get_prob(const QTables& Q_table_, const state_t& state, const action_t& action, const action_mask_t& possible_actions) const {
    action_mask_t max_actions = Q_table_.greedy_actions(state, possible_actions);

    float prob = 0.0f;

    if (max_actions[action]) {
        prob += (1.f - epsilon_) / max_actions.count();
    } else if (possible_actions[action]) {
        prob = epsilon_ / possible_actions.count();
    }

    return prob;
};