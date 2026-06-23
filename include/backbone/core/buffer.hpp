#ifndef _BUFFER_HPP_
#define _BUFFER_HPP_

#include <deque>
#include <vector>
#include "common.hpp"

struct transition {
    state_t s_;
    action_t a_;
    float r_;
    state_t next_s_;
    // for sarsa
    action_t next_a_ = 0;
    // for off-policy method
    float log_rho_ = 0.0f;
    // for off-policy sarsa
    float next_log_rho_ = 0.0f;
    bool done_;
    bool timeout_ = false;
    action_mask_t s_possible_actions;
    action_mask_t next_s_possible_actions;
    
};

class buffer {
protected:
    std::deque<transition> data_;

public:
    virtual ~buffer() = default;

    virtual std::vector<transition> pop() = 0;
    virtual bool ready() const = 0;

    void push_back(const transition& data) {
        data_.push_back(data);
    }

    // implement this to throw out all the leftover values
    std::vector<transition> flush() {
        auto ret = std::vector<transition>(data_.begin(), data_.end());
        data_.clear();
        return ret;
    }
};

#endif