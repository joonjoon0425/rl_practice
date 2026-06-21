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

enum class algoType {TD, EXPECTED_TD, MC};
enum class policyType {ON_POLICY, OFF_POLICY};

std::shared_ptr<agent> create_agent(int state_size, int action_size, algoType a, policyType p, float epsilon, float gamma = 0.9f, float alpha = 0.5f, float init = 0.0f) {
    std::shared_ptr<policy> t_p = nullptr;
    std::shared_ptr<policy> b_p = nullptr;
    std::unique_ptr<buffer> buffer = nullptr;
    std::unique_ptr<updater> updater = nullptr;
    
    if (a == algoType::TD) {
        if (p == policyType::ON_POLICY) {
            t_p = std::make_shared<epsilonGreedyPolicy>(epsilon);
            b_p = t_p;
            buffer = std::make_unique<nStepBuffer>(1);
            updater = std::make_unique<sarsaUpdater>();
        } else if (p == policyType::OFF_POLICY) {
            t_p = std::make_shared<greedyPolicy>();
            b_p = std::make_shared<epsilonGreedyPolicy>(epsilon);
            buffer = std::make_unique<nStepBuffer>(1);
            updater = std::make_unique<QLearningUpdater>();
        }
    } else if (a == algoType::EXPECTED_TD) {

    } else if (a == algoType::MC) {

    }

    return std::make_shared<agent>(state_size, action_size, t_p, b_p, std::move(buffer), std::move(updater), gamma, alpha, init);
}

#endif