#ifndef _Q_TABLE_HPP_
#define _Q_TABLE_HPP_

#include <cassert>
#include <vector>
#include "core/common.hpp"

using state_t = int;
using action_t = int;

class QTables {
private:
    std::vector<float> Q_table_;
    int table_num_;
    int state_size_;
    int action_size_;

public:
    QTables(int state_size, int action_size, float init = 0.0f, int table_num = 1);
    float& operator()(state_t state, action_t action, int table = -1);
    float operator()(state_t state, action_t action, int table = -1) const;
    
    action_t greedy_action(const state_t& state, const action_mask_t& possible_actions) const;
    action_mask_t greedy_actions(const state_t& state, const action_mask_t& possible_actions) const;

    float max(const state_t& state, const action_mask_t& possible_actions) const;

    int index(const state_t& state, const action_t& action) {
        return state * action_size_ + action;
    }
};

#endif