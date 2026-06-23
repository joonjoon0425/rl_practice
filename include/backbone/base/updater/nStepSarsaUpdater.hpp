#ifndef _N_STEP_SARSA_UPDATER_HPP_
#define _N_STEP_SARSA_UPDATER_HPP_

#include <core/updater.hpp>

class nStepSarsaUpdater : public updater {
public:
    void update(QValueSource &Q_table, const std::vector<transition> &data, float gamma, float alpha) override;
};

#endif