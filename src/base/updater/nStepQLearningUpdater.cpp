#include <base/updater/nStepQLearningUpdater.hpp>

void nStepQLearningUpdater::update(QValueSource &Q_table, const std::vector<transition> &data, float gamma, float alpha) {
    int len = data.size();
    bool done = data[len - 1].done_;
    state_t last_s = data[len - 1].next_s_;
    float G = done ? 0.0f : Q_table.max(last_s, data[len - 1].next_s_possible_actions);
    state_t s = data[0].s_;
    action_t a = data[0].a_;


    for (int i = len - 1; i >= 0; i--) {
        G = data[i].r_ + gamma * G;
    }

    Q_table(s, a) += alpha * (G - Q_table(s, a));
}