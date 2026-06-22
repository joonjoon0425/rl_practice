#ifndef _MONTE_CARLO_BUFFER_HPP_
#define _MONTE_CARLO_BUFFER_HPP_

#include <core/buffer.hpp>

class monteCarloBuffer : public buffer {
public:
    bool ready() const override;
    std::vector<transition> pop() override;
};

#endif 