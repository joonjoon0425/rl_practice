#ifndef _BUFFER_HPP_
#define _BUFFER_HPP_

#include <deque>
#include <vector>
#include <set>

using state_t = int;
using action_t = int;

struct transition {
    state_t s_;
    action_t a_;
    float reward_;
    state_t next_s_;
    bool done_;
    std::vector<bool> possible_actions;

    // for sarsa
    action_t next_a_;
    // for off-policy method
    float rho_ = 1.0f;
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

    // implement this to throw all the leftover values
    void clear() { data_.clear(); }
};

#endif