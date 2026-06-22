#ifndef _REGISTER_HPP_
#define _REGISTER_HPP_

#include "core/agent.hpp"

#include "base/policy/greedyPolicy.hpp"
#include "base/policy/epsilonGreedyPolicy.hpp"

#include "base/buffer/nStepBuffer.hpp"
#include "base/buffer/monteCarloBuffer.hpp"

#include <base/updater/sarsaUpdater.hpp>
#include <base/updater/QLearningUpdater.hpp>
#include <base/updater/monteCarloUpdater.hpp>
#include <base/updater/expectedSarsaUpdater.hpp>

#include "core/schedular.hpp"

#include <memory>

enum class algoType {sarsa, QLearning, firstVisitOnPolicyMC, everyVisitOnPolicyMC, offPolicyMC, expectedSarsa};

std::shared_ptr<agent> create_agent(int state_size, int action_size, algoType a, float epsilon, float gamma = 0.9f, float alpha = 0.5f, float init = 0.0f) {
    std::shared_ptr<policy> t_p = nullptr;
    std::shared_ptr<policy> b_p = nullptr;
    std::unique_ptr<buffer> buffer = nullptr;
    std::unique_ptr<updater> updater = nullptr;
    
    switch (a) {
        case algoType::sarsa:
            t_p = std::make_shared<epsilonGreedyPolicy>(epsilon);
            b_p = t_p;
            buffer = std::make_unique<nStepBuffer>(1);
            updater = std::make_unique<sarsaUpdater>();
            break;
        case algoType::QLearning:
            t_p = std::make_shared<greedyPolicy>();
            b_p = std::make_shared<epsilonGreedyPolicy>(epsilon);
            buffer = std::make_unique<nStepBuffer>(1);
            updater = std::make_unique<QLearningUpdater>();
            break;
        case algoType::firstVisitOnPolicyMC:
            t_p = std::make_shared<epsilonGreedyPolicy>(epsilon);
            b_p = t_p;
            buffer = std::make_unique<monteCarloBuffer>();
            updater = std::make_unique<onPolicyMonteCarloUpdater>(false, state_size, action_size);
            break;
        case algoType::everyVisitOnPolicyMC:
            t_p = std::make_shared<epsilonGreedyPolicy>(epsilon);
            b_p = t_p;
            buffer = std::make_unique<monteCarloBuffer>();
            updater = std::make_unique<onPolicyMonteCarloUpdater>(true, state_size, action_size);
            break;
        case algoType::offPolicyMC:
            t_p = std::make_shared<greedyPolicy>();
            b_p = std::make_shared<epsilonGreedyPolicy>(epsilon);
            buffer = std::make_unique<monteCarloBuffer>();
            updater = std::make_unique<offPolicyMonteCarloUpdater>(state_size, action_size);
            break;
        case algoType::expectedSarsa:
            t_p = std::make_shared<greedyPolicy>();
            b_p = std::make_shared<epsilonGreedyPolicy>(epsilon);
            buffer = std::make_unique<nStepBuffer>(1);
            updater = std::make_unique<expectedSarsaUpdater>(t_p);
            break;
    }


    return std::make_shared<agent>(state_size, action_size, t_p, b_p, std::move(buffer), std::move(updater), gamma, alpha, init);
}

#endif