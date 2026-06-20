#ifndef _GREEDY_POLICY_HPP_
#define _GREEDY_POLICY_HPP_

#include "core/policy.hpp"
#include <set>
#include <cstdlib>
#include <limits>

template <typename Env>
class greedy_policy : public policy<Env> {
public:
    greedy_policy(Env& env) : policy<Env>(env) {}

    Env::action_t get_action(const std::vector<float>& Q_table_, const Env::state_t& state) const override {
        std::set<typename Env::action_t> actions = this->env_.get_possible_actions(state);
        float max = -std::numeric_limits<float>::infinity();
        std::vector<typename Env::action_t> max_actions;

        for (typename Env::action_t act: actions) {
            float q = Q_table_[this->env_.idx(state, act)];
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

    float get_prob(const std::vector<float>& Q_table_, const Env::state_t& state, const Env::action_t& action) const override {
        std::set<typename Env::action_t> actions = this->env_.get_possible_actions(state);
        float max = -std::numeric_limits<float>::infinity();
        std::vector<typename Env::action_t> max_actions;

        for (typename Env::action_t act: actions) {
            float q = Q_table_[this->env_.idx(state, act)];
            if (q > max) {
                max = q;
                max_actions.clear();
                max_actions.push_back(act);
            } else if (q == max) {
                max_actions.push_back(act);
            }
        }

        return 1.f / max_actions.size();
    };
};

#endif