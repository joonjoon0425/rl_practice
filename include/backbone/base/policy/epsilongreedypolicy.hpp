#ifndef _EPSILON_GREEDY_POLICY_HPP_
#define _EPSILON_GREEDY_POLICY_HPP_

#include "core/policy.hpp"
#include "core/qtable.hpp"
#include <set>

class epsilonGreedyPolicy : public policy, public epsilonSchedulable {
protected:
    float epsilon_;

public:
    epsilonGreedyPolicy(float epsilon);

    float& epsilon() override {return epsilon_;}
    float epsilon() const override {return epsilon_;}
    void epsilon(float val) override {epsilon_ = val;}

    action_t get_action(const QTables& Q_table_, const state_t& state, const std::vector<bool>& possible_actions) const override;
    

    action_t greedy_action(const QTables& Q_table_, const state_t& state, const std::vector<bool>& possible_actions) const;

    action_t random_action(const state_t& cur, const std::vector<bool>& possible_actions) const;

    float get_prob(const QTables& Q_table_, const state_t& state, const action_t& action, const std::vector<bool>& possible_actions) const override;
};

#endif