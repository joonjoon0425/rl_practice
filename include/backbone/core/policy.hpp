#ifndef _POLICY_HPP_
#define _POLICY_HPP_

#include <vector>
#include <set>
#include "schedulable.hpp"
#include "qtable.hpp"

using state_t = int;
using action_t = int;

class policy {
protected:
    mutable std::vector<action_t> buffer;

public:
    virtual ~policy() = default;

    virtual action_t get_action(const QTables& Q_table_, const state_t& state, const std::vector<bool>& possible_actions) const = 0;
    virtual float get_prob(const QTables& Q_table_, const state_t& state, const action_t& action, const std::vector<bool>& possible_actions) const = 0;
};


#endif