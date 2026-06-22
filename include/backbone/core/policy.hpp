#ifndef _POLICY_HPP_
#define _POLICY_HPP_

#include <vector>
#include <set>
#include "schedulable.hpp"
#include "QValueSource.hpp"
#include "common.hpp"

using state_t = int;
using action_t = int;

class policy {
protected:
    mutable std::vector<action_t> buffer;

public:
    virtual ~policy() = default;

    virtual action_t get_action(const QValueSource& Q_table_, const state_t& state, const action_mask_t& possible_actions) const = 0;
    virtual float get_prob(const QValueSource& Q_table_, const state_t& state, const action_t& action, const action_mask_t& possible_actions) const = 0;

    action_t random_action(const state_t& cur, const action_mask_t& possible_actions) const;
};


#endif