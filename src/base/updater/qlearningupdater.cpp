#include "base/updater/qlearningupdater.hpp"
#include <limits>

QLearningUpdater::QLearningUpdater(){}

void QLearningUpdater::update(QTables& Q_table, const std::vector<transition>& data, float gamma, float alpha) {
    // requires two transition data
    assert(data.size() == 1 && "qlearning requires one update data");
    
    state_t s = data[0].s_;
    action_t a = data[0].a_;
    float reward = data[0].reward_;
    state_t s_next = data[0].next_s_;
    bool done = data[0].done_;

    float q_m = q_max(Q_table, s_next, data[0].possible_actions);
    float q_next = done ? 0.0f : q_m;
    float target = reward + gamma * q_next;
    Q_table(s, a) += alpha * (target - Q_table(s, a));
}

// helper
float QLearningUpdater::q_max(const QTables& Q_table, const state_t& state, const std::vector<bool>& possible_actions) {
    float max = -std::numeric_limits<float>::infinity();

    for (int i = 0; i < possible_actions.size(); i++) {
        if (possible_actions[i]) {
            float q = Q_table(state, i);
            max = max < q ? q : max; 
        }
    }

    return max;
}