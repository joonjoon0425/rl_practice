#ifndef _DOUBLE_QLERNING_UPDATER_HPP_
#define _DOUBLE_QLERNING_UPDATER_HPP_

#include <core/updater.hpp>

class doubleQLearningUpdater : public updater {
public:
    void update(QValueSource &Q_table, const std::vector<transition> &data, float gamma, float alpha) override;
};

#endif