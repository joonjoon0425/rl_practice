#include "core/agent.hpp"
#include "core/QValueSource.hpp"
#include <cmath>
#include <memory>

agent::agent(int state_size, int action_size,
    std::shared_ptr<policy> t_p,
    std::shared_ptr<policy> b_p,
    std::unique_ptr<buffer> buffer,
    std::unique_ptr<updater> updater,
    std::unique_ptr<QValueEstimator> estimator,
    float gamma, float alpha, float init,
    int table_num)
:
    target_policy_(t_p),
    behavior_policy_(b_p),
    buffer_(std::move(buffer)),
    updater_(std::move(updater)),
    q_tables_(state_size, action_size, init, table_num, std::move(estimator)),
    gamma_(gamma),
    alpha_(alpha)
{}

void agent::observe(transition data) {
    // calculate rho_ when off policy method is used
    if (behavior_policy_ != target_policy_) {
        data.log_rho_ = std::log(target_policy_->get_prob(q_tables_, data.s_, data.a_, data.s_possible_actions))
        - std::log(behavior_policy_->get_prob(q_tables_, data.s_, data.a_, data.s_possible_actions));

        assert(data.log_rho_ != std::numeric_limits<float>::infinity() && "log_rho_ is +inf, this action cannot be chosen by behavior policy.");
    }

    buffer_->push_back(data);
    if (buffer_->ready()) {
        //std::cout << "DEBUG: READY FOR UPDATE\n";
        updater_->update(q_tables_, buffer_->pop(), gamma_, alpha_);
    }
}

action_t agent::sample_action(const state_t& state, const action_mask_t& possible_actions) {
    if (behavior_policy_) return behavior_policy_->get_action(q_tables_, state, possible_actions);
    return target_policy_->get_action(q_tables_, state, possible_actions);
}

action_t agent::predict_action(const state_t& state, const action_mask_t& possible_actions) {
    return target_policy_->get_action(q_tables_, state, possible_actions);
}

action_t agent::random_action(const state_t& state, const action_mask_t& possible_actions) {
    action_mask_internal_t mask = possible_actions.to_ullong();
    assert(mask > 0 && "no actions");
    int size = possible_actions.count();

    int rand = std::rand() % size;

    for (int i = 0; i < rand; i++) mask &= (mask - 1);

    assert(mask != 0 && "mask became zero unexpectedly");
    return __builtin_ctzll(mask);
}

// debug?
action_t agent::greedy_action(const state_t& state, const action_mask_t& possible_actions) {
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
float agent::max_q(const state_t& state, const action_mask_t& possible_actions) {
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
    auto leftover = buffer_->flush();
    if (leftover.empty() || leftover.back().timeout_) return;
    else updater_->update(q_tables_, leftover, gamma_, alpha_);
}

