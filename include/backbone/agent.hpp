#ifndef _AGENT_HPP_
#define _AGENT_HPP_

template <typename State, typename Action, typename Transition>
class agent {
public:
    virtual Action get_action(State) = 0;
    virtual void update(const Transition&) = 0;
};

#endif