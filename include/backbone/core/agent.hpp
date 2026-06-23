#ifndef _AGENT_HPP_
#define _AGENT_HPP_

#include "buffer.hpp"
#include "core/QValueSource.hpp"
#include "policy.hpp"
#include "updater.hpp"
#include "QValueSource.hpp"
#include "common.hpp"

#include <cassert>
#include <functional>
#include <memory>
#include <limits>

class agent {
protected:
    QValueSource q_tables_;

    std::shared_ptr<policy> target_policy_;
    std::shared_ptr<policy> behavior_policy_;
    std::unique_ptr<buffer> buffer_;
    std::unique_ptr<updater> updater_;

    float gamma_;
    float alpha_;

    std::function<action_mask_t(const state_t&)> get_action_mask_;

public:
    agent(int state_size, int action_size,
        std::function<action_mask_t(const state_t&)> get_action_mask,
        std::shared_ptr<policy> t_p,
        std::shared_ptr<policy> b_p,
        std::unique_ptr<buffer> buffer,
        std::unique_ptr<updater> updater,
        std::unique_ptr<QValueEstimator> estimator,
        float gamma, float alpha,
        float init, int table_num);

    void observe(transition data);

    action_t sample_action(const state_t& state);
    action_t predict_action(const state_t& state);
    action_t random_action(const state_t& state);

    // debug?
    action_t greedy_action(const state_t& state);
    // debug
    float max_q(const state_t& state);

    
    void flush_buffer();

    //getter and setter
    float& gamma() {return gamma_;}
    float& alpha() {return alpha_;}
    std::shared_ptr<policy> target_policy() {return target_policy_;}
    std::shared_ptr<policy> behavior_policy() {return behavior_policy_;};
    QValueSource& Qtable(){ return q_tables_;}
    

};


#endif