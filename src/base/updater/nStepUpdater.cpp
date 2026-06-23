#include "core/QValueSource.hpp"
#include <base/updater/nStepUpdater.hpp>
#include <cmath>

void nStepUpdater::update(QValueSource &Q_table, const std::vector<transition> &data, float gamma, float alpha) {
    this->update_fn(Q_table, data, gamma, alpha);
}

void nStepUpdater::onPolicyUpdate(QValueSource &Q_table, const std::vector<transition> &data, float gamma, float alpha, bool expected) {
    int len = data.size();
    bool done = data[len - 1].done_;
    float G = 0.0f;
    if (!done) {
        if (!expected) G = Q_table(data[len - 1].next_s_, data[len - 1].next_a_);
        else {
            for (int i = 0; i < Q_table.action_size(); i++)
                G += Q_table(data[len - 1].next_s_, i) * target_policy_->get_prob(Q_table, data[len - 1].next_s_, i, data[len - 1].next_s_possible_actions);
        }
    }
    
    for (int i = len - 1; i >= 0; i--) {
        G = data[i].r_ + gamma * G;
    }

    Q_table(data[0].s_, data[0].a_) += alpha * (G - Q_table(data[0].s_, data[0].a_));
}

void nStepUpdater::offPolicyUpdate(QValueSource &Q_table, const std::vector<transition> &data, float gamma, float alpha, bool expected) {
    int len = data.size();
    bool done = data[len - 1].done_;
    float G = 0.0f;
    float log_rho = 0.0f;
    state_t s = data[0].s_;
    action_t a = data[0].a_;
    
    if(!done) {
        if (!expected) {
            G = Q_table(data[len - 1].next_s_, data[len - 1].next_a_);
            log_rho += std::log(target_policy_->get_prob(Q_table, data[len - 1].next_s_, data[len - 1].next_a_, data[len - 1].next_s_possible_actions)) - data[len - 1].next_log_b_prob_;
        } else {
            for (int i = 0; i < Q_table.action_size(); i++)
                G += Q_table(data[len - 1].next_s_, i) * target_policy_->get_prob(Q_table, data[len - 1].next_s_, i, data[len - 1].next_s_possible_actions);
        }
    }

    for (int i = len - 1; i >= 0; i--) {
        G = data[i].r_ + gamma * G;
        if (i > 0) log_rho += std::log(target_policy_->get_prob(Q_table, data[i].s_, data[i].a_, data[i].s_possible_actions)) - data[i].log_b_prob_;
    }

    Q_table(s, a) += alpha * std::exp(log_rho) * (G - Q_table(s, a));
}