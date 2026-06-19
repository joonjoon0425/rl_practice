#ifndef _Q_AGENT_HPP_
#define _Q_AGENT_HPP_

#include <agent.hpp>
#include <vector>
#include <set>
#include <limits>
#include <cstdlib>

template<typename Env>
struct q_transition {
    Env::state_t s;
    Env::action_t a;
    float r;
    Env::state_t s_next;
    bool done;
};

template <typename Env>
class q_agent : public agent<Env, q_transition<Env>> {
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
    
    void update(const q_transition<Env>& t) override {
        float next_q = t.done ? 0.0f : max_q(t.s_next);
        float target = t.r + next_q;
        Q_table_[idx(t.s, t.a)] += alpha_ * (target - Q_table_[idx(t.s,t.a)]);
    }

    Env::action_t greedy_action(const Env::state_t& state) const {
        std::set<typename Env::action_t> actions = Env::get_possible_actions(state);
        float max = -std::numeric_limits<float>::infinity();
        std::vector<typename Env::action_t> max_actions;

        for (typename Env::action_t act: actions) {
            float q = Q_table_[idx(state, act)];
            if (q > max) {
                max = q;
                max_actions.clear();
                max_actions.push_back(act);
            } else if (q == max) {
                max_actions.push_back(act);
            }
        }

        return max_actions[std::rand() % max_actions.size()];
    }

    Env::action_t random_action(const Env::state_t& cur) const {
        std::set<typename Env::action_t> actions = Env::get_possible_actions(cur);
        int rand_idx = std::rand() % actions.size();
        return *std::next(actions.begin(), rand_idx);
    }

    float max_q(const Env::state_t& cur) const {
        std::set<typename Env::action_t> actions = Env::get_possible_actions(cur);
        float max = -std::numeric_limits<float>::infinity();

        for (auto act: actions) {
            if (Q_table_[idx(cur, act)] > max) {
                max = Q_table_[idx(cur, act)];
            }
        }

        return max;
    }

    virtual int idx(const Env::state_t& state, const Env::action_t& act) const = 0;
};

#endif