#ifndef _ENV_HPP_
#define _ENV_HPP_

#include <tuple>


template <typename State, typename Action>
class environment {
public:
    virtual void reset(bool random) = 0;
    virtual std::tuple<State, float, bool> step(Action) = 0;
};

#endif