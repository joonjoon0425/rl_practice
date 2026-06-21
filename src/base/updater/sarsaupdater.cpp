#include "base/updater/sarsaupdater.hpp"
#include <cassert>

sarsaUpdater::sarsaUpdater(){}

void sarsaUpdater::update(QTables& Q_table, const std::vector<transition>& data, float gamma, float alpha) {
    // requires one transition data
    assert(data.size() == 1 && "sarsa requires one update data");
    
    state_t s = data[0].s_;
    action_t a = data[0].a_;
    float reward = data[0].reward_;
    state_t s_next = data[0].next_s_;
    action_t a_next = data[0].next_a_;
    bool done = data[0].done_;
    
    float q_next = done ? 0.0f : Q_table(s_next, a_next);
    float target = reward + gamma * q_next;
    Q_table(s, a) += alpha * (target - Q_table(s, a));
}