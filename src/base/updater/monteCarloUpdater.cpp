#include <base/updater/monteCarloUpdater.hpp>
#include <unordered_set>
#include <limits>
#include <cmath>

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
    float log_W = 0.0f;

    for (int i = len - 1; i >= 0; i--) {
        if (std::isinf(log_W) && log_W < 0) break;
        assert(!std::isnan(log_W) && "log_W became NaN");

        int saIdx = Q_table.index(data[i].s_, data[i].a_);
        G = gamma * G + data[i].reward_;
        float W = std::exp(log_W);
        C_[saIdx] += W;
        Q_table(data[i].s_, data[i].a_) += W / C_[saIdx] * (G - Q_table(data[i].s_, data[i].a_));
        
        assert(!std::isnan(data[i].log_rho_) && "log_rho_ is NaN");
        assert(data[i].log_rho_ != std::numeric_limits<float>::infinity() && "log_rho_ is +inf, this action cannot be chosen by behavior policy.");

        log_W += data[i].log_rho_;
    }
}
