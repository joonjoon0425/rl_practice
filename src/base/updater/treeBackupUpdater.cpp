#include <base/updater/treeBackupUpdater.hpp>

void treeBackupUpdater::update(QValueSource &Q_table, const std::vector<transition> &data, float gamma, float alpha) {
    int len = data.size();
    bool done = data[len - 1].done_;
    float G = data[len - 1].r_;

    if(!done) {
        float expec = 0.0f;
        for (int i = 0; i < Q_table.action_size(); i++) {
            expec += target_policy_->get_prob(Q_table, data[len - 1].next_s_, i, data[len - 1].next_s_possible_actions) * Q_table(data[len - 1].next_s_, i);
        }
        G += gamma * expec;
    }

    for (int i = len - 2; i >= 0; i--) {
        float expec = 0.0f;
        for (int j = 0; j < Q_table.action_size(); j++) {
            if (j != data[i + 1].a_)
            expec += target_policy_->get_prob(Q_table, data[i].next_s_, j, data[i].next_s_possible_actions) * Q_table(data[i].next_s_, j);
        }
        G = data[i].r_ + gamma * expec + gamma * G *target_policy_->get_prob(Q_table, data[i].next_s_, data[i + 1].a_, data[i].next_s_possible_actions);
    }

    Q_table(data[0].s_, data[0].a_) += alpha * (G - Q_table(data[0].s_, data[0].a_));
}