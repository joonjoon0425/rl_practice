#ifndef _REGISTER_HPP_
#define _REGISTER_HPP_

#include "base/buffer/nStepBuffer.hpp"
#include "base/policy/greedyPolicy.hpp"
#include <core/core.hpp>
#include <base/base.hpp>

#include <memory>

enum class algoType {
    sarsa,
    QLearning,
    firstVisitOnPolicyMC,
    everyVisitOnPolicyMC,
    offPolicyMC,
    offPolicyExpectedSarsa,
    onPolicyExpectedSarsa,
    doubleQLearning,
    nStepSarsa,
    offPolicyNStepExpectedSarsa,
    onPolicyNStepExpectedSarsa
};

std::shared_ptr<agent> create_agent(int state_size, int action_size, std::function<action_mask_t(const state_t&)> get_action_mask, algoType a, float epsilon, float gamma = 0.9f, float alpha = 0.2f, float init = 0.0f) {
    std::shared_ptr<policy> t_p = nullptr;
    std::shared_ptr<policy> b_p = nullptr;
    std::unique_ptr<buffer> buffer = nullptr;
    std::unique_ptr<updater> updater = nullptr;
    std::unique_ptr<QValueEstimator> estimator = nullptr;
    int table_num = 1;
    
    switch (a) {
        case algoType::sarsa:
            t_p = std::make_shared<epsilonGreedyPolicy>(epsilon);
            b_p = t_p;
            buffer = std::make_unique<nStepBuffer>(1);
            updater = std::make_unique<sarsaUpdater>();
            break;
        case algoType::QLearning:
        case algoType::offPolicyExpectedSarsa:
            t_p = std::make_shared<greedyPolicy>();
            b_p = std::make_shared<epsilonGreedyPolicy>(epsilon);
            buffer = std::make_unique<nStepBuffer>(1);
            updater = std::make_unique<QLearningUpdater>();
            break;
        case algoType::doubleQLearning:
            t_p = std::make_shared<greedyPolicy>();
            b_p = std::make_shared<epsilonGreedyPolicy>(epsilon);
            buffer = std::make_unique<nStepBuffer>(1);
            updater = std::make_unique<doubleQLearningUpdater>();
            estimator = std::make_unique<arithmeticMeanEstimator>();
            table_num = 2;
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
        case algoType::onPolicyExpectedSarsa:
            t_p = std::make_shared<epsilonGreedyPolicy>(epsilon);
            b_p = t_p;
            buffer = std::make_unique<nStepBuffer>(1);
            updater = std::make_unique<expectedSarsaUpdater>(t_p);
            break;
        case algoType::nStepSarsa:
            t_p = std::make_shared<epsilonGreedyPolicy>(epsilon);
            b_p = t_p;
            buffer = std::make_unique<nStepBuffer>(5);
            updater = std::make_unique<nStepSarsaUpdater>();
            break;
        case algoType::onPolicyNStepExpectedSarsa:
            t_p = std::make_shared<epsilonGreedyPolicy>(epsilon);
            b_p = t_p;
            buffer = std::make_unique<nStepBuffer>(5);
            updater = std::make_unique<nStepExpectedSarsaUpdater>(t_p);
            break;
        case algoType::offPolicyNStepExpectedSarsa:
            t_p = std::make_shared<greedyPolicy>();
            b_p = std::make_shared<epsilonGreedyPolicy>(epsilon);
            buffer = std::make_unique<nStepBuffer>(5);
            updater = std::make_unique<nStepExpectedSarsaUpdater>(t_p);
            break;
    }


    return std::make_shared<agent>(state_size, action_size, get_action_mask, t_p, b_p, std::move(buffer), std::move(updater), std::move(estimator), gamma, alpha, init, table_num);
}

#endif