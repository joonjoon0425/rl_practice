#ifndef _BUFFER_HPP_
#define _BUFFER_HPP_

#include <deque>
#include <vector>

template <typename Env>
struct transition {
    Env::state_t s_;
    Env::action_t a_;
    float reward_;
    Env::state_t next_s_;

    bool done_;

    // for off-policy method
    float rho_ = 1.0f;
};

template <typename Env>
class buffer {
protected:
    std::deque<transition<Env>> data_;

public:
    virtual ~buffer() = default;
    // virtual std::vector<transition<Env>> pop_front() = 0;

    virtual std::vector<transition<Env>> pop() = 0;
    virtual bool ready() const = 0;
    // void push_front(const transition<Env>& data) {
    //     data_.push_front(data);
    // }

    void push_back(const transition<Env>& data) {
        data_.push_back(data);
    }


    
};

#endif