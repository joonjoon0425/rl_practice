#include <base/updater/nStepSarsaUpdater.hpp>

void nStepSarsaUpdater::update(QValueSource &Q_table, const std::vector<transition> &data, float gamma, float alpha) {
    int len = data.size();
    bool done = data[len - 1].done_;
    float G = done ? 0.0f : Q_table(data[len - 1].next_s_, data[len - 1].next_a_);

    for (int i = len - 1; i >= 0; i--) {
        G = data[i].r_ + gamma * G;
    }

    Q_table(data[0].s_, data[0].a_) += alpha * (G - Q_table(data[0].s_, data[0].a_));
}