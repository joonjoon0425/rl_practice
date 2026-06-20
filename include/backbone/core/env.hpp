#ifndef _ENV_HPP_
#define _ENV_HPP_

#include <tuple>
#include <set>
#include <concepts>

template <typename Env>
concept Environment = requires (Env env, typename Env::state_t s, typename Env::action_t a) {
    typename Env::state_t;
    typename Env::action_t;

    {env.state_to_index(s)} -> std::convertible_to<std::size_t>;
    {env.action_to_index(a)} -> std::convertible_to<std::size_t>;

    {env.state_size()} -> std::convertible_to<std::size_t>;
    {env.action_size()} -> std::convertible_to<std::size_t>;

    {env.get_possible_actions(s)} -> std::convertible_to<std::set<typename Env::action_t>>;
};


template <typename State, typename Action, typename Derived>
class environment {
protected:
    int state_size_;
    int action_size_;

public:
    using state_t = State;
    using action_t = Action;
    virtual state_t reset(bool random) = 0;
    virtual std::tuple<State, float, bool> step(const state_t&, const action_t&) = 0;

    environment(int s, int a) : state_size_(s), action_size_(a) {}

    std::set<action_t> get_possible_actions(const state_t& state) const {
        return static_cast<const Derived*>(this)->impl_get_possible_actions(state);
    }

    int state_to_index(const state_t& state) const {
        return static_cast<const Derived*>(this)->impl_state_to_index(state);
    }

    int action_to_index(const action_t& action) const {
        return static_cast<const Derived*>(this)->impl_action_to_index(action);
    }

    int idx(const state_t& state, const action_t& action) const {
        return state_to_index(state) * action_size_ + action_to_index(action);
    }

    int state_size() const {return state_size_;}
    int action_size() const {return action_size_;}
};

#endif