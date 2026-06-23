#ifndef _N_STEP_Q_LEARNING_UPDATER_HPP_
#define _N_STEP_Q_LEARNING_UPDATER_HPP_

#include "core/updater.hpp"
class nStepQLearningUpdater : public updater {
public:
    void update(QValueSource &Q_table, const std::vector<transition> &data, float gamma, float alpha) override;
};

#endif