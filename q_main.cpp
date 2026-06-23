#include "core/schedulable.hpp"
#include "core/schedular.hpp"
#include <cassert>
#include <cliffwalking.hpp>
#include <memory>
#include <register.hpp>
#include <print>
//임시방편 데이터 수집
struct PerformanceMetrics {
    std::vector<float> reward_history;
    std::vector<int> step_history;
    long long total_cumulative_steps = 0;
};


void print_policy_map(std::shared_ptr<agent> agent, const cliffWalking::cliff2D& env);

int main() {
    int w = 15;
    int h = 15;
    float init = 0.0f;

    cliffWalking::cliff2D env(w, h, {0, 0}, {12,2}, 
        {{4, 13}, {5, 13}, {6, 13}, {7, 13}, {8, 13}, {9, 13}, {10, 13}, {11, 13}, {12, 13}, {13, 13}, {14, 13},
               {4, 12}, {4, 11}, {4, 10}, {4, 9}, {4, 8}, {4, 7}, {4, 6}, {4, 5}, {4, 4}, {4, 3}, {4, 2}, {4, 1},
            {5, 2}, {6, 3}, {7, 4}, {8, 5}, {9,6},{10,7},{11,8},{12,9},{13,10},
                {13,9},{13,8},{13,7},{13,6},{13,5},{13,4},{13,3},{13,2},{13,1},
            {12, 1},{11,1},{10,1},{9,1},{8,1},{7,1}});
    // QLEARN
    auto agent = create_agent(env.state_size(), env.action_size(), [&](const state_t& s){return env.get_possible_actions(s);}, algoType::offPolicyNStepExpectedSarsa, 1.0, 0.9, 0.001, 3);
    auto ptr = std::dynamic_pointer_cast<epsilonSchedulable>(agent->behavior_policy());
    assert(ptr != nullptr && "dynamic cast failure");

    schedular<float> eps_exp_sche((ptr->epsilon()),
        [](float val, int _) {
            float ret = val * 0.999954f;
            return ret < 0.01f ? 0.01f : ret;
        }
    );

    //임시방편 데이터 수집
    PerformanceMetrics metrics;

    int episodes = 1000000;

    for (int i = 0; i < episodes; i++) {
        auto cur = env.reset(true);

        bool terminate = false;
        int total_steps = 0;
        float total_reward = 0.f;

        while (!terminate) {
            auto a = agent->sample_action(cur);
            auto [next_s, reward, done] = env.step(cur, a);
            agent->observe({
                .s_ = cur,
                .a_ = a,
                .r_ = reward,
                .next_s_ = next_s,
                .done_ = done
            });

            cur = next_s;
            terminate = done;
            
            total_steps++;
            total_reward += reward;
            //임시방편
            metrics.reward_history.push_back(total_reward);
            metrics.step_history.push_back(total_steps);
            metrics.total_cumulative_steps += total_steps;
        }
        eps_exp_sche.step(i);

        agent->flush_buffer();
        

        if ((i + 1) % 10000 == 0) {
            // 최근 1000개 에피소드의 이동 평균 계산
            float avg_reward = 0;
            int start_idx = std::max(0, i - 1000);
            for(int k = start_idx; k <= i; ++k) avg_reward += metrics.reward_history[k];
            avg_reward /= (i - start_idx + 1);

            std::print("EPISODE {}: Moving Avg Reward = {:.2f}, Cumulative Steps = {}\n", 
               i + 1, avg_reward, metrics.total_cumulative_steps);
}
    }
    std::print("epsilon: {}\n",eps_exp_sche.value());
    print_policy_map(agent, env);

    //debug
    for(int j = 0; j < env.state_size(); j++) {
        for (int i = 0; i < env.action_size(); i++) {
            
        }
    }
}

void print_policy_map(std::shared_ptr<agent> agent, const cliffWalking::cliff2D& env) {
    const char* symbol[] = {"↑", "↓", "←", "→"};
    cliffWalking::cliff2D::envState_t p{0, 0};
    
    // ANSI 코드를 상수문자열로 정의
    const char* RED   = "\033[31m";
    const char* BLUE  = "\033[34m";
    const char* RESET = "\033[0m";

    for (int y = env.height() - 1; y >= 0; y--) {
        for (int x = 0; x < env.width(); x++) {
            p.x_ = x;
            p.y_ = y;
            
            if (env.traps().find(p) != env.traps().end()) {
                // 붉은색 T 출력 후 리셋
                std::print("{}{:<3}{}", RED, "T", RESET); 
            } else if (p == env.goal()) {
                // 푸른색 G 출력 후 리셋
                std::print("{}{:<3}{}", BLUE, "G", RESET); 
            } else {
                int action_idx = static_cast<int>(agent->greedy_action(env.state_to_index(p)));
                std::print("{:<3}", symbol[action_idx]);
            }
        }
        std::print("\n");
    }
}