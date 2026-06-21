#ifndef _NSTEPBUFFER_HPP_
#define _NSTEPBUFFER_HPP_

#include <core/env.hpp>
#include <core/buffer.hpp>

class nStepBuffer : public buffer {
protected:
    int steps_;
public:
    nStepBuffer(int steps);

    std::vector<transition> pop() override;

    bool ready() const override;
};

#endif