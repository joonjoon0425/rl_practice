#ifndef _Q_AGENT_HPP_
#define _Q_AGENT_HPP_

#include <agent.hpp>
#include <vector>

template<typename Env>
struct q_transition {
    Env::state_t s;
    Env::action_t a;
    float r;
    Env::state_t s_next;
    bool done;
};

template <typename Env>
class q_agent : public agent<typename Env::state_t, typename Env::action_t, q_transition<Env>> {
protected:
    std::vector<float> Q_table_;
    size_t state_size_;
    size_t action_size_;

    float alpha_;
    float gamma_;

public:
    q_agent(size_t state_size, size_t action_size, float alpha, float gamma, float init)
    : state_size_(state_size),
      action_size_(action_size),
      alpha_(alpha),
      gamma_(gamma),
      Q_table_(state_size * action_size, init) {}
    
    void update(const q_transition<Env>&) override {

    }

    Env::action_t greedy_action(const Env::state_t& state) const {

    }

    Env::action_t random_action(const Env::state_t&) const {

    }

    float max_q(const Env::state& cur) const {
        
    }

    


};

#endif