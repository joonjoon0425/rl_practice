#ifndef _AGENT_HPP_
#define _AGENT_HPP_

#include "env.hpp"
#include "buffer.hpp"
#include "policy.hpp"
#include "updater.hpp"

#include <cassert>
#include <iostream>
#include <memory>


template <Environment Env>
class agent {
protected:
    Env& env_;
    std::vector<float> Q_table_;

    std::shared_ptr<policy<Env>> target_policy_;
    std::shared_ptr<policy<Env>> behavior_policy_;
    std::unique_ptr<buffer<Env>> buffer_;
    std::unique_ptr<updater<Env>> updater_;

    float gamma_;
    float alpha_;

public:
    agent(Env& env, std::shared_ptr<policy<Env>> t_p, std::shared_ptr<policy<Env>> b_p, std::unique_ptr<buffer<Env>> buffer, std::unique_ptr<updater<Env>> updater, float gamma, float alpha, float init)
    : env_(env),
      target_policy_(t_p),
      behavior_policy_(b_p),
      buffer_(std::move(buffer)),
      updater_(std::move(updater)),
      Q_table_(env.state_size() * env.action_size(), init),
      gamma_(gamma),
      alpha_(alpha)
    {}

    void observe(const transition<Env>& data) {
        buffer_->push_back(data);
        if (buffer_->ready()) {
            //std::cout << "DEBUG: READY FOR UPDATE\n";
            updater_->update(Q_table_, buffer_->pop(), gamma_, alpha_);
        }
    }

    Env::action_t sample_action(const Env::state_t& state) {
        if (behavior_policy_) return behavior_policy_->get_action(Q_table_, state);
        return target_policy_->get_action(Q_table_, state);
    }

    Env::action_t predict_action(const Env::state_t& state) {
        return target_policy_->get_action(Q_table_, state);
    }

    // debug?
    Env::action_t greedy_action(const Env::state_t& state) {
        std::set<typename Env::action_t> actions = this->env_.get_possible_actions(state);
        float max = -std::numeric_limits<float>::infinity();
        std::vector<typename Env::action_t> max_actions;

        for (typename Env::action_t act: actions) {
            float q = Q_table_[this->env_.idx(state, act)];
            if (q > max) {
                max = q;
                max_actions.clear();
                max_actions.push_back(act);
            } else if (q == max) {
                max_actions.push_back(act);
            }
        }

        return max_actions[std::rand() % max_actions.size()];
    }

    void flush_buffer() {
        buffer_->clear();
    }

    float& gamma() {return gamma_;}
    float& alpha() {return alpha_;}

    std::shared_ptr<policy<Env>> target_policy() {return target_policy_;}
    std::shared_ptr<policy<Env>> behavior_policy() {return behavior_policy_;};
    
    // debug
    float max_q(const Env::state_t& state) {
        std::set<typename Env::action_t> actions = this->env_.get_possible_actions(state);
        float max = -std::numeric_limits<float>::infinity();

        for (typename Env::action_t act: actions) {
            float q = Q_table_[this->env_.idx(state, act)];
            if (q > max) max = q;
        }

        return max;
    }

};


#endif