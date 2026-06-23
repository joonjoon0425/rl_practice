#include "core/agent.hpp"
#include "core/QValueSource.hpp"
#include <cmath>
#include <memory>

agent::agent(int state_size, int action_size,
    std::function<action_mask_t(const state_t&)> get_action_mask,
    std::shared_ptr<policy> t_p,
    std::shared_ptr<policy> b_p,
    std::unique_ptr<buffer> buffer,
    std::unique_ptr<updater> updater,
    std::unique_ptr<QValueEstimator> estimator,
    float gamma, float alpha, float init,
    int table_num)
:
    get_action_mask_(get_action_mask),
    target_policy_(t_p),
    behavior_policy_(b_p),
    buffer_(std::move(buffer)),
    updater_(std::move(updater)),
    q_tables_(state_size, action_size, init, table_num, std::move(estimator)),
    gamma_(gamma),
    alpha_(alpha)
{}

void agent::observe(transition data) {
    // fill in s_possible_actions and next_s_possible_actions
    data.s_possible_actions = get_action_mask_(data.s_);
    data.next_s_possible_actions = get_action_mask_(data.next_s_);

    // THIS WAS A MISTAKE!!!
    // THIS IS WRONG ----> calculate rho_ when off policy method is used
    // SINCE WE SHOULD CACULATE TARGET PROBABILITY OF UPDATE TIME STEP, PRE-CACULATION OF RHO IS WRONG.
    // NOW ALMOST ALL UPDATER SHOULD HAVE TARGET POLICY
    if (behavior_policy_ != target_policy_) {
        data.log_b_prob_ = std::log(behavior_policy_->get_prob(q_tables_, data.s_, data.a_, data.s_possible_actions));
    // only for n step off policy sarsa
        data.next_log_b_prob_ = std::log(behavior_policy_->get_prob(q_tables_, data.next_s_, data.next_a_, data.next_s_possible_actions));

        // assert(data.log_rho_ != std::numeric_limits<float>::infinity() && "log_rho_ is +inf, this action cannot be chosen by behavior policy.");
    }

    buffer_->push_back(data);
    if (buffer_->ready()) {
        //std::cout << "DEBUG: READY FOR UPDATE\n";
        updater_->update(q_tables_, buffer_->pop(), gamma_, alpha_);
    }
}

action_t agent::sample_action(const state_t& state) {
    auto possible_actions = get_action_mask_(state);
    if (behavior_policy_) return behavior_policy_->get_action(q_tables_, state, possible_actions);
    return target_policy_->get_action(q_tables_, state, possible_actions);
}

action_t agent::predict_action(const state_t& state) {
    auto possible_actions = get_action_mask_(state);
    return target_policy_->get_action(q_tables_, state, possible_actions);
}

action_t agent::random_action(const state_t& state) {
    auto possible_actions = get_action_mask_(state);
    action_mask_internal_t mask = possible_actions.to_ullong();
    assert(mask > 0 && "no actions");
    int size = possible_actions.count();

    int rand = std::rand() % size;

    for (int i = 0; i < rand; i++) mask &= (mask - 1);

    assert(mask != 0 && "mask became zero unexpectedly");
    return __builtin_ctzll(mask);
}

// debug?
action_t agent::greedy_action(const state_t& state) {
    auto possible_actions = get_action_mask_(state);
    return q_tables_.greedy_action(state, possible_actions);
}
// debug
float agent::max_q(const state_t& state) {
    auto possible_actions = get_action_mask_(state);
    return q_tables_.max(state, possible_actions);
}

void agent::flush_buffer() {
    auto leftover = buffer_->flush();
    if (leftover.empty() || leftover.back().timeout_) return;

    for (int i = 0; i < leftover.size(); i++) {
        updater_->update(q_tables_, std::vector<transition>(leftover.begin() + i, leftover.end()), gamma_, alpha_);
    }
}

