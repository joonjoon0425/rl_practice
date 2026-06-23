#ifndef _N_STEP_EXPECTED_SARSA_UPDATER_HPP_
#define _N_STEP_EXPECTED_SARSA_UPDATER_HPP_

#include <core/updater.hpp>
#include <core/policy.hpp>
#include <memory>

class nStepExpectedSarsaUpdater : public updater {
private:
    std::shared_ptr<policy> target_policy_;

public:
    nStepExpectedSarsaUpdater(std::shared_ptr<policy> target_policy) : target_policy_(target_policy) {}

    void update(QValueSource &Q_table, const std::vector<transition> &data, float gamma, float alpha) override;
};

#endif