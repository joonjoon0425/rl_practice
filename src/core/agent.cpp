#include "core/agent.hpp"

agent::agent(int state_size, int action_size,
    std::shared_ptr<policy> t_p,
    std::shared_ptr<policy> b_p,
    std::unique_ptr<buffer> buffer,
    std::unique_ptr<updater> updater, float gamma,
    float alpha, float init,
    int table_num)
:
    target_policy_(t_p),
    behavior_policy_(b_p),
    buffer_(std::move(buffer)),
    updater_(std::move(updater)),
    q_tables_(state_size, action_size, init, table_num),
    gamma_(gamma),
    alpha_(alpha)
{}

void agent::observe(const transition& data) {
    buffer_->push_back(data);
    if (buffer_->ready()) {
        //std::cout << "DEBUG: READY FOR UPDATE\n";
        updater_->update(q_tables_, buffer_->pop(), gamma_, alpha_);
    }
}

action_t agent::sample_action(const state_t& state, const std::vector<bool>& possible_actions) {
    if (behavior_policy_) return behavior_policy_->get_action(q_tables_, state, possible_actions);
    return target_policy_->get_action(q_tables_, state, possible_actions);
}

action_t agent::predict_action(const state_t& state, const std::vector<bool>& possible_actions) {
    return target_policy_->get_action(q_tables_, state, possible_actions);
}

// debug?
action_t agent::greedy_action(const state_t& state, const std::vector<bool>& possible_actions) {
    float max = -std::numeric_limits<float>::infinity();
    std::vector<action_t> buffer;

    for (int i = 0; i < possible_actions.size(); i++) {
        if (possible_actions[i]) {
            float q = q_tables_(state, i);
            if (q > max) {
                max = q;
                buffer.clear();
                buffer.push_back(i);
            } else if (max - q < 1e-7f) {
                buffer.push_back(i);
            }
        }
    }

    return buffer[std::rand() % buffer.size()];
}
// debug
float agent::max_q(const state_t& state, const std::vector<bool>& possible_actions) {
    float max = -std::numeric_limits<float>::infinity();

    for (int i = 0; i < possible_actions.size(); i++) {
        if (possible_actions[i]) {
            float q = q_tables_(state, i);
            max = max < q ? q : max; 
        }
    }

    return max;
}

void agent::flush_buffer() {
    buffer_->clear();
}

