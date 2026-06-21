#ifndef _AGENT_HPP_
#define _AGENT_HPP_

#include "buffer.hpp"
#include "policy.hpp"
#include "updater.hpp"
#include "qtable.hpp"

#include <cassert>
#include <memory>
#include <limits>

using state_t = int;
using action_t = int;

class agent {
protected:
    QTables q_tables_;

    std::shared_ptr<policy> target_policy_;
    std::shared_ptr<policy> behavior_policy_;
    std::unique_ptr<buffer> buffer_;
    std::unique_ptr<updater> updater_;

    float gamma_;
    float alpha_;

public:
    agent(int state_size, int action_size,
        std::shared_ptr<policy> t_p,
        std::shared_ptr<policy> b_p,
        std::unique_ptr<buffer> buffer,
        std::unique_ptr<updater> updater,
        float gamma, float alpha,
        float init = 0.0f, int table_num = 1);

    void observe(const transition& data);

    action_t sample_action(const state_t& state, const std::vector<bool>& possible_actions);
    action_t predict_action(const state_t& state, const std::vector<bool>& possible_actions);

    // debug?
    action_t greedy_action(const state_t& state, const std::vector<bool>& possible_actions);
    // debug
    float max_q(const state_t& state, const std::vector<bool>& possible_actions);

    
    void flush_buffer();

    //getter and setter
    float& gamma() {return gamma_;}
    float& alpha() {return alpha_;}
    std::shared_ptr<policy> target_policy() {return target_policy_;}
    std::shared_ptr<policy> behavior_policy() {return behavior_policy_;};
    
    

};


#endif