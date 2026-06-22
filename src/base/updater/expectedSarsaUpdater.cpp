#include <base/updater/expectedSarsaUpdater.hpp>

void expectedSarsaUpdater::update(QValueSource& Q_table, const std::vector<transition>& data, float gamma, float alpha) {
    // requires one transition data
    assert(data.size() == 1 && "sarsa requires one update data");
    
    state_t s = data[0].s_;
    action_t a = data[0].a_;
    float reward = data[0].reward_;
    state_t s_next = data[0].next_s_;
    bool done = data[0].done_;
    
    float value_expec = 0.0f;
    if (!done) {
        for (int i = 0; i < Q_table.action_size(); i++) {
            value_expec += Q_table(s_next, i) * target_policy_->get_prob(Q_table, s_next, i, data[0].possible_actions);
        }
    }

    float target = reward + gamma * value_expec;
    Q_table(s, a) += alpha * (target - Q_table(s, a));
}