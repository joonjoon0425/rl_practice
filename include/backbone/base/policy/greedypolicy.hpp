#ifndef _GREEDY_POLICY_HPP_
#define _GREEDY_POLICY_HPP_

#include "core/policy.hpp"
#include <set>

class greedyPolicy : public policy{
public:
    greedyPolicy();

    action_t get_action(const QTables& Q_table_, const state_t& state, const std::vector<bool>& possible_actions) const override;

    float get_prob(const QTables& Q_table_, const state_t& state, const action_t& action, const std::vector<bool>& possible_actions) const override;
};

#endif