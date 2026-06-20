#ifndef _QLEARNING_UPDATER_HPP_
#define _QLEARNING_UPDATER_HPP_

#include "core/updater.hpp"
#include <cassert>
#include <set>
#include <cassert>
#include <limits>

template <typename Env>
class qlearning_updater : public updater<Env> {
public:
    qlearning_updater(Env& env) : updater<Env>(env) {}

    void update(std::vector<float> Q_table, const std::vector<transition<Env>>& data, float gamma, float alpha) override {
        // requires two transition data
        assert(data.size() == 1 && "qlearning requires one update data");
        
        typename Env::state_t s = data[0].s_;
        typename Env::action_t a = data[0].a_;
        float reward = data[0].reward_;
        typename Env::state_t s_next = data[0].next_s_;
        bool done = data[0].done_;

        int cur_index = this->env_.idx(s, a);

        float q_m = q_max(Q_table, s_next);
        float q_next = done ? 0.0f : q_m;
        float target = reward + gamma * q_next;
        Q_table[cur_index] += alpha * (target - Q_table[cur_index]);
    }

    // helper
    float q_max(const std::vector<float>& Q_table, const Env::state_t& state) {
        std::set<typename Env::action_t> actions = this->env_.get_possible_actions(state);
        float max = -std::numeric_limits<float>::infinity();

        for (typename Env::action_t act: actions) {
            float q = Q_table[this->env_.idx(state, act)];
            if (q > max) max = q;
        }

        return max;
    }
};

#endif