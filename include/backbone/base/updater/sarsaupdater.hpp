#ifndef _SARSA_UPDATER_HPP_
#define _SARSA_UPDATER_HPP_

#include "core/updater.hpp"
#include <cassert>

template <typename Env>
class sarsa_updater : public updater<Env> {
public:
    sarsa_updater(Env& env) : updater<Env>(env) {}

    void update(std::vector<float>& Q_table, const std::vector<transition<Env>>& data, float gamma, float alpha) override {
        // requires two transition data
        assert(data.size() == 1 && "sarsa requires one update data");
        
        typename Env::state_t s = data[0].s_;
        typename Env::action_t a = data[0].a_;
        float reward = data[0].reward_;
        typename Env::state_t s_next = data[0].next_s_;
        typename Env::action_t a_next = data[0].next_a_;
        bool done = data[0].done_;

        int cur_index = this->env_.idx(s, a);
        int next_index = this->env_.idx(s_next, a_next);
        
        float q_next = done ? 0.0f : Q_table[next_index];
        float target = reward + gamma * q_next;
        Q_table[cur_index] += alpha * (target - Q_table[cur_index]);
    }
};

#endif