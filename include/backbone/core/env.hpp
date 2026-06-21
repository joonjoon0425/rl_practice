#ifndef _ENV_HPP_
#define _ENV_HPP_

#include <tuple>
#include <concepts>
#include <vector>

#include "common.hpp"


template <typename Env>
concept Environment = requires (Env env, typename Env::state_t s, typename Env::action_t a) {
    typename Env::state_t;
    typename Env::action_t;

    {env.state_to_index(s)} -> std::convertible_to<std::size_t>;
    {env.action_to_index(a)} -> std::convertible_to<std::size_t>;

    {env.state_size()} -> std::convertible_to<std::size_t>;
    {env.action_size()} -> std::convertible_to<std::size_t>;

    {env.get_possible_actions(s)} -> std::convertible_to<std::vector<bool>>;
};


template <typename State, typename Action, typename Derived>
class environment {
protected:
    int state_size_;
    int action_size_;

public:
    using envState_t = State;
    using envAction_t = Action;
    
    virtual state_t reset(bool random) = 0;

    virtual std::tuple<state_t, float, bool, action_mask_t> step(const state_t&, const action_t&) = 0;

    environment(int s, int a) : state_size_(s), action_size_(a) {}

    action_mask_t get_possible_actions(const state_t& state) const {
        return static_cast<const Derived*>(this)->impl_get_possible_actions(state);
    }

    int state_to_index(const envState_t& state) const {
        return static_cast<const Derived*>(this)->impl_state_to_index(state);
    }

    int action_to_index(const envAction_t& action) const {
        return static_cast<const Derived*>(this)->impl_action_to_index(action);
    }

    envState_t index_to_state(int index) const {
        return static_cast<const Derived*>(this)->impl_index_to_state(index);
    }

    envAction_t index_to_action(int index) const {
        return static_cast<const Derived*>(this)->impl_index_to_action(index);
    }

    int state_size() const {return state_size_;}
    int action_size() const {return action_size_;}
};

#endif