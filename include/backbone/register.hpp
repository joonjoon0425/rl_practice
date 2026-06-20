#ifndef _REGISTER_HPP_
#define _REGISTER_HPP_

#include "core/agent.hpp"

#include "base/policy/greedypolicy.hpp"
#include "base/policy/epsilongreedypolicy.hpp"

#include "base/buffer/nstepbuffer.hpp"

#include "base/updater/sarsaupdater.hpp"
#include "base/updater/qlearningupdater.hpp"

#include "core/schedular.hpp"

#include <memory>

enum class algo_type {TD, EXPECTED_TD, MC};
enum class policy_type {ON_POLICY, OFF_POLICY};

template <typename Env>
std::shared_ptr<agent<Env>> create_agent(Env& env, algo_type a, policy_type p, float epsilon, float gamma = 0.9f, float alpha = 0.5f, float init = 0.0f) {
    std::shared_ptr<policy<Env>> t_p = nullptr;
    std::shared_ptr<policy<Env>> b_p = nullptr;
    std::unique_ptr<buffer<Env>> buffer = nullptr;
    std::unique_ptr<updater<Env>> updater = nullptr;
    
    if (a == algo_type::TD) {
        if (p == policy_type::ON_POLICY) {
            t_p = std::make_shared<epsilon_greedy_policy<Env>>(env, epsilon);
            b_p = t_p;
            buffer = std::make_unique<nstep_buffer<Env>>(1);
            updater = std::make_unique<sarsa_updater<Env>>(env);
        } else if (p == policy_type::OFF_POLICY) {
            t_p = std::make_shared<greedy_policy<Env>>(env);
            b_p = std::make_shared<epsilon_greedy_policy<Env>>(env, epsilon);
            buffer = std::make_unique<nstep_buffer<Env>>(1);
            updater = std::make_unique<qlearning_updater<Env>>(env);
        }
    } else if (a == algo_type::EXPECTED_TD) {

    } else if (a == algo_type::MC) {

    }

    return std::make_shared<agent<Env>>(env, t_p, b_p, std::move(buffer), std::move(updater), gamma, alpha, init);
}

#endif