#ifndef _MONTE_CARLO_AGENT_HPP_
#define _MONTE_CARLO_AGENT_HPP_

#include <env.hpp>
#include <set>
#include <cstdlib>
#include <limits>
#include "agent.hpp"
#include <unordered_set>
#include <vector>
#include <iostream>

template <typename Env>
struct monte_carlo_transition {
    std::vector<typename Env::state_t> states;
    std::vector<typename Env::action_t> actions;
    std::vector<float> rewards;
};

template <typename Env>
class monte_carlo_agent : agent<Env, monte_carlo_transition<Env>> {
protected:
    std::vector<float> Q_table_;
    std::vector<int> N_table_;

    int state_size_;
    int action_size_;

    bool every_visit_;

    float gamma_;
    float epsilon_;

public:
    monte_carlo_agent(int state_size, int action_size, float epsilon, float gamma, bool every_visit, float init = 0.f)
    : state_size_(state_size),
      action_size_(action_size),
      every_visit_(every_visit),
      gamma_(gamma),
      epsilon_(epsilon),
      Q_table_(state_size * action_size, init),
      N_table_(state_size * action_size, 0) {};

    float& epsilon() {return epsilon_;}

    void update(const monte_carlo_transition<Env>& t) override {
        std::vector<float> G(t.actions.size() + 1, 0);
        int total_steps = t.actions.size();

        std::vector<int> seen(state_size_ * action_size_, 0);
        
        typename Env::state_t cur_state = t.states[0];
        typename Env::action_t cur_action;
        float cur_reward;

        for (int i = total_steps - 1; i >= 0; i--) G[i] = gamma_ * G[i + 1] + t.rewards[i];

        for (int i = 0; i < total_steps; i++) {
            cur_state = t.states[i];
            cur_action = t.actions[i];
            int cur_idx = idx(cur_state, cur_action);
            if (every_visit_ || seen[cur_idx] == 0) {
                N_table_[cur_idx]++;
                Q_table_[cur_idx] += (G[i] - Q_table_[cur_idx]) / N_table_[cur_idx];
                seen[cur_idx] = 1;
            }
        }

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

    virtual int idx(const Env::state_t& state, const Env::action_t& act) const = 0;
};

#endif