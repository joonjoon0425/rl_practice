#ifndef _ENV_HPP_
#define _ENV_HPP_

#include <tuple>
#include <set>

template <typename State, typename Action, typename Derived>
class environment {
public:
    using state_t = State;
    using action_t = Action;
    virtual void reset(bool random) = 0;
    virtual std::tuple<State, float, bool> step(Action) = 0;

    static std::set<action_t> get_possible_actions(const state_t& state) {
        return Derived::impl_get_possible_actions(state);
    }
};

#endif