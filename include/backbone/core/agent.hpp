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

    std::unique_ptr<policy<Env>> target_policy_;
    std::unique_ptr<policy<Env>> behavior_policy_;
    std::unique_ptr<buffer<Env>> buffer_;
    std::unique_ptr<updater<Env>> updater_;

    float gamma_;
    float alpha_;

public:
    agent(Env& env, std::unique_ptr<policy<Env>> t_p, std::unique_ptr<policy<Env>> b_p, std::unique_ptr<buffer<Env>> buffer, std::unique_ptr<updater<Env>> updater, float gamma, float alpha, float init)
    : env_(env),
      target_policy_(std::move(t_p)),
      behavior_policy_(std::move(b_p)),
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

    void flush_buffer() {
        buffer_->clear();
    }

};


#endif