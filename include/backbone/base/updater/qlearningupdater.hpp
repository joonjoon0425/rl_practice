#ifndef _QLEARNING_UPDATER_HPP_
#define _QLEARNING_UPDATER_HPP_

#include "core/updater.hpp"
#include <cassert>
#include <set>
#include <cassert>

class QLearningUpdater : public updater {
public:
    QLearningUpdater();

    void update(QTables& Q_table, const std::vector<transition>& data, float gamma, float alpha) override;

    // helper
    float q_max(const QTables& Q_table, const state_t& state, const std::vector<bool>& possible_actions);
};

#endif