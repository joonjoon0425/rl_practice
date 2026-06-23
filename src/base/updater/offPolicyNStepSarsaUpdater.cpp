#include <base/updater/offPolicyNStepSarsaUpdater.hpp>
#include <cmath>

void offPolicyNStepSarsaUpdater::update(QValueSource &Q_table, const std::vector<transition> &data, float gamma, float alpha) {
    int len = data.size();
    state_t s = data[0].s_;
    action_t a = data[0].a_;
    
    bool done = data[len - 1].done_;
    float log_rho = done ? 0.0f : data[len - 1].next_log_rho_;
    float G = done ? 0.0f : Q_table(data[len - 1].next_s_, data[len - 1].next_a_);

    for (int i = len - 1; i >= 0; i--) {
        log_rho += data[i].log_rho_;
        G = data[i].r_ + gamma * G;
    }

    Q_table(s, a) += alpha * std::exp(log_rho) * (G - Q_table(s, a));
}