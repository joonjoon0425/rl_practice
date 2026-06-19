#ifndef _AGENT_HPP_
#define _AGENT_HPP_

template <typename Env, typename Transition>
class agent {
public:
    virtual Env::action_t get_action(const Env::state_t&) const = 0;
    virtual void update(const Transition&) = 0;
};

#endif