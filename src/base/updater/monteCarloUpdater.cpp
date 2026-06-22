#include <base/updater/monteCarloUpdater.hpp>
#include <unordered_set>

void onPolicyMonteCarloUpdater::update(
    QValueSource &Q_table,
    const std::vector<transition> &data,
    float gamma, float alpha
) {
    int len = data.size();
    std::vector<float> G(len + 1, 0);
    std::unordered_set<int> seen;

    for (int i = len - 1; i >= 0; i--) {
        G[i] = gamma * G[i + 1] + data[i].reward_;
    }

    for (int i = 0; i < len; i++) {
        int saIdx = Q_table.index(data[i].s_, data[i].a_);
        if (every_visit_ || seen.find(saIdx) == seen.end()) {
            N_[saIdx]++;
            Q_table(data[i].s_, data[i].a_) += (G[i] - Q_table(data[i].s_, data[i].a_)) / N_[saIdx];
            seen.insert(saIdx);
        }
    }
}

void offPolicyMonteCarloUpdater::update(
    QValueSource &Q_table,
    const std::vector<transition> &data,
    float gamma, float alpha
) {
    int len = data.size();
    float G = 0;
    float W = 1;

    for (int i = len - 1; i >= 0; i--) {
        if (W == 0) break;
        int saIdx = Q_table.index(data[i].s_, data[i].a_);
        G = gamma * G + data[i].reward_;
        C_[saIdx] += W;
        Q_table(data[i].s_, data[i].a_) += W / C_[saIdx] * (G - Q_table(data[i].s_, data[i].a_));
        W *= data[i].rho_;
    }
}
