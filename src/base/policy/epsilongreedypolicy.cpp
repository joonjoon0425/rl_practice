#include "base/policy/epsilongreedypolicy.hpp"
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

action_t epsilonGreedyPolicy::random_action(const state_t& state, const action_mask_t& possible_actions) const {
    action_mask_internal_t mask = possible_actions.to_ullong();
    assert(mask > 0 && "no actions");
    int size = possible_actions.count();

    int rand = std::rand() % size;

    for (int i = 0; i < rand; i++) mask &= (mask - 1);

    assert(mask != 0 && "mask became zero unexpectedly");
    return __builtin_ctzll(mask);
}

float epsilonGreedyPolicy::get_prob(const QTables& Q_table_, const state_t& state, const action_t& action, const action_mask_t& possible_actions) const {
    action_mask_t max_actions = Q_table_.greedy_actions(state, possible_actions);

    float prob = epsilon_ / possible_actions.count();

    if (max_actions[action]) {
        prob += (1.f - epsilon_) / max_actions.count();
    }

    return prob;
};