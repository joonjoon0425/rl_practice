#ifndef _NSTEPBUFFER_HPP_
#define _NSTEPBUFFER_HPP_

#include <core/env.hpp>
#include <core/buffer.hpp>
#include <iostream>

template <Environment Env>
class nstep_buffer : public buffer<Env> {
protected:
    int steps_;
public:
    nstep_buffer(int steps) : steps_(steps) {
        
    }

    std::vector<transition<Env>> pop() override {
        auto ret_data = std::vector<transition<Env>>(this->data_.begin(), this->data_.begin() + steps_);
        this->data_.pop_front();
        return ret_data;
    }

    bool ready() const override {
        if (this->data_.size() >= steps_) {
            //std::cout << "DEBUG: READY FOR UPDATE\n";
            return true;
        }
        return false;
    }
};

#endif