#include <base/updater/doubleQLearningUpdater.hpp>

void doubleQLearningUpdater::update(QValueSource &Q_table, const std::vector<transition> &data, float gamma, float alpha) {
    assert(Q_table.table_size() == 2 && "Q table size is not 2");
    state_t s = data[0].s_;
    action_t a = data[0].a_;
    float reward = data[0].r_;
    state_t next_s = data[0].next_s_;
    bool done = data[0].done_;
    
    bool table_idx = std::rand() % 2;
    
    float q = 0.0f;
    if (!done) {
        action_t greedy_act = Q_table.greedy_action(next_s, data[0].next_s_possible_actions, table_idx);
        q = Q_table(next_s, greedy_act, !table_idx);
    }

    float target = reward + gamma * q;

    Q_table(s, a, table_idx) += alpha * (target - Q_table(s, a, table_idx));
}