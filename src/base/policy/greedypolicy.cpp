#include "base/policy/greedypolicy.hpp"
#include <cstdlib>
#include <limits>

greedyPolicy::greedyPolicy(){}

action_t greedyPolicy::get_action(const QTables& Q_table_, const state_t& state, const std::vector<bool>& possible_actions) const {
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

float greedyPolicy::get_prob(const QTables& Q_table_, const state_t& state, const action_t& action, const std::vector<bool>& possible_actions) const {
    float max = -std::numeric_limits<float>::infinity();
    std::vector<bool> max_actions(possible_actions.size());
    int max_actions_size = 0;

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
        }
    }

    return float(max_actions[state]) / max_actions_size;
};