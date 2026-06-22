#ifndef _QLEARNING_UPDATER_HPP_
#define _QLEARNING_UPDATER_HPP_

#include "core/updater.hpp"
#include <cassert>
#include "core/common.hpp"
#include <cassert>

class QLearningUpdater : public updater {
public:
    QLearningUpdater();
    void update(QTables& Q_table, const std::vector<transition>& data, float gamma, float alpha) override;
};

#endif