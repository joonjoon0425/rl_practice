#ifndef _N_STEP_UPDATER_HPP_
#define _N_STEP_UPDATER_HPP_

#include <core/updater.hpp>
#include <functional>
#include <memory>
#include <core/policy.hpp>

class nStepUpdater : public updater {
private:
    std::function<void(QValueSource &, const std::vector<transition> &, float, float)> update_fn;
    std::shared_ptr<policy> target_policy_;

public:
    nStepUpdater(std::shared_ptr<policy> target_policy, bool onPolicy, bool expected) 
    : target_policy_(target_policy) 
    {
        if (onPolicy){
            update_fn = [this, expected](QValueSource &Q_table, const std::vector<transition> &data, float gamma, float alpha) {
                this->onPolicyUpdate(Q_table, data, gamma, alpha, expected);
            };
        } else {
            update_fn = [this, expected](QValueSource &Q_table, const std::vector<transition> &data, float gamma, float alpha) {
                this->offPolicyUpdate(Q_table, data, gamma, alpha, expected);
            };
        }
    }

    void update(QValueSource &Q_table, const std::vector<transition> &data, float gamma, float alpha) override;

    void onPolicyUpdate(QValueSource &Q_table, const std::vector<transition> &data, float gamma, float alpha, bool expected);
    void offPolicyUpdate(QValueSource &Q_table, const std::vector<transition> &data, float gamma, float alpha, bool expected);
};

#endif