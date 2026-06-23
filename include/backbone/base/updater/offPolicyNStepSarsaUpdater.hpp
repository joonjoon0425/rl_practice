#ifndef _OFF_POLICY_N_STEP_UPDATER_HPP_
#define _OFF_POLICY_N_STEP_UPDATER_HPP_

#include "core/updater.hpp"

class offPolicyNStepSarsaUpdater : public updater {
public:
    void update(QValueSource &Q_table, const std::vector<transition> &data, float gamma, float alpha) override;
};

#endif