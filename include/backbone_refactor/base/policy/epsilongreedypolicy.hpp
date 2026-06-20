#ifndef _EPSILON_GREEDY_POLICY_HPP_
#define _EPSILON_GREEDY_POLICY_HPP_

#include "core/policy.hpp"
#include <set>
#include <cstdlib>
#include <limits>

template <typename Env>
class epsilon_greedy_policy : public policy<Env> {
protected:
    float epsilon_;

public:
    epsilon_greedy_policy(Env& env, float epsilon) : policy<Env>(env), epsilon_(epsilon) {}

    float& epsilon() {return epsilon_;}

    Env::action_t get_action(const std::vector<float>& Q_table_, const Env::state_t& state) const override {
        typename Env::action_t ret_action;
        float rand_val = static_cast<float>(std::rand()) / RAND_MAX;

        if (rand_val < epsilon_) {
            //explore
            ret_action = random_action(state);
        } else {
            //exploit
            ret_action = greedy_action(Q_table_, state);
        }

        return ret_action;
    }

    Env::action_t greedy_action(const std::vector<float>& Q_table_, const Env::state_t& state) const {
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

    Env::action_t random_action(const Env::state_t& cur) const {
        std::set<typename Env::action_t> actions = this->env_.get_possible_actions(cur);
        int rand_idx = std::rand() % actions.size();
        return *std::next(actions.begin(), rand_idx);
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

        return 1 + epsilon_ * (1 / actions.size()  - 1 / max_actions.size());
    };
};

#endif