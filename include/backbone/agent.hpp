#ifndef _AGENT_HPP_
#define _AGENT_HPP_

template <typename State, typename Action>
class agent {
public:
    virtual Action get_action(State) = 0;
    virtual void update(const State&, const Action&, float, const State&, const Action&) = 0;
};

#endif