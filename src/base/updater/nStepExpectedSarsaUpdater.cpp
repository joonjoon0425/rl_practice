#include "core/common.hpp"
#include <base/updater/nStepExpectedSarsaUpdater.hpp>

void nStepExpectedSarsaUpdater::update(QValueSource &Q_table, const std::vector<transition> &data, float gamma, float alpha) {
    int len = data.size();
    bool done = data[len - 1].done_;
    float G = 0.0f;
    float log_rho = 0.0f;
    state_t s = data[0].s_;
    action_t a = data[0].a_;
    
    state_t last_s = data[len - 1].s_;
    action_mask_t last_s_possible_actions = data[len - 1].s_possible_actions;
    
    if(!done) {
        for (int i = 0; i < Q_table.action_size(); i++)
            G += Q_table(last_s, i) * target_policy_->get_prob(Q_table, last_s, i, last_s_possible_actions);
    }

    for (int i = len - 1; i >= 0; i--) {
        G = data[i].r_ + gamma * G;
        log_rho += data[i].log_rho_;
    }

    Q_table(s, a) += alpha * log_rho * (G - Q_table(s, a));
}