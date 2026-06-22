#ifndef _EPSILON_GREEDY_POLICY_HPP_
#define _EPSILON_GREEDY_POLICY_HPP_

#include "core/policy.hpp"
#include "core/common.hpp"

class epsilonGreedyPolicy : public policy, public epsilonSchedulable {
protected:
    float epsilon_;

public:
    epsilonGreedyPolicy(float epsilon);

    float& epsilon() override {return epsilon_;}
    float epsilon() const override {return epsilon_;}
    void epsilon(float val) override {epsilon_ = val;}

    action_t get_action(const QValueSource& Q_table_, const state_t& state, const action_mask_t& possible_actions) const override;
    
    action_t greedy_action(const QValueSource& Q_table_, const state_t& state, const action_mask_t& possible_actions) const;

    float get_prob(const QValueSource& Q_table_, const state_t& state, const action_t& action, const action_mask_t& possible_actions) const override;
};

#endif