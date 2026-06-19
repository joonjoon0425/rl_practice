#ifndef _ENV_HPP_
#define _ENV_HPP_

#include <tuple>
#include <set>

template <typename State, typename Action>
class environment {
    using state_t = State;
    using action_t = Action;
public:
    virtual void reset(bool random) = 0;
    virtual std::tuple<State, float, bool> step(Action) = 0;

    // static std::set<action_t> get_possible_actions(state_t);
};

#endif