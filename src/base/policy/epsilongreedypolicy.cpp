#include "base/policy/epsilongreedypolicy.hpp"
#include <algorithm>
#include <cstdlib>
#include <limits>

epsilonGreedyPolicy::epsilonGreedyPolicy(float epsilon) : epsilon_(epsilon) {}

action_t epsilonGreedyPolicy::get_action(const QTables& Q_table_, const state_t& state, const std::vector<bool>& possible_actions) const {
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

action_t epsilonGreedyPolicy::greedy_action(const QTables& Q_table_, const state_t& state, const std::vector<bool>& possible_actions) const {
    float max = -std::numeric_limits<float>::infinity();
    buffer.clear();

    for (int i = 0; i < possible_actions.size(); i++) {
        if (possible_actions[i]) {
            float q = Q_table_(state, i);
            if (q > max) {
                max = q;
                buffer.clear();
                buffer.push_back(i);
            } else if (max - q < 1e-7f) {
                buffer.push_back(i);
            }
        }
    }

    return buffer[std::rand() % buffer.size()];
}

action_t epsilonGreedyPolicy::random_action(const state_t& cur, const std::vector<bool>& possible_actions) const {
    buffer.clear();

    for (int i = 0; i < possible_actions.size(); i++) {
        if (possible_actions[i]) buffer.push_back(i);
    }

    return buffer[std::rand() % buffer.size()];
}

float epsilonGreedyPolicy::get_prob(const QTables& Q_table_, const state_t& state, const action_t& action, const std::vector<bool>& possible_actions) const {
    float max = -std::numeric_limits<float>::infinity();
    std::vector<bool> max_actions(possible_actions.size());
    int max_actions_size = 0;
    int possible_actions_size = 0;

    for (int i = 0; i < possible_actions.size(); i++) {
        if (possible_actions[i]) {
            float q = Q_table_(state, i);
            if (q > max) {
                max = q;
                max_actions.assign(max_actions.size(), false);
                max_actions[i] = true;
                max_actions_size = 1;
            } else if (max - q < 1e-7f) {
                max_actions[i] = true;
                max_actions_size++;
            }
            possible_actions_size++;
        }
    }

    float prob = epsilon_ / possible_actions_size;

    if (max_actions[action]) {
        prob += (1.f - epsilon_) / max_actions_size;
    }

    return prob;
};