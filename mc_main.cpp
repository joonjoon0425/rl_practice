#include "core/schedulable.hpp"
#include "core/schedular.hpp"
#include <cassert>
#include <cliffwalking.hpp>
#include <limits>
#include <memory>
#include <register.hpp>
#include <print>

void print_policy_map(std::shared_ptr<agent> agent, const cliffWalking::cliff2D& env);

int main() {
    int w = 15;
    int h = 15;
    float init = 0.0f;

    cliffWalking::cliff2D env(w, h, {0, 0}, {14,14}, {{10, 13}, {11, 13}, {12, 13}, {13, 13}, {14, 13}});
    // SARSA
    auto agent = create_agent(env.state_size(), env.action_size(), [&](const state_t& s){return env.get_possible_actions(s);}, algoType::offPolicyMC, 1.0, 0.9f, 0.001);
    auto ptr = std::dynamic_pointer_cast<epsilonSchedulable>(agent->behavior_policy());
    assert(ptr != nullptr && "dynamic cast failure");

    schedular<float> eps_exp_sche((ptr->epsilon()),
        [](float val, int _) {
            float ret = val * 0.999954f;
            return ret < 0.1f ? 0.1f : ret;
        }
    );

    int episodes = 1000000;

    int max_stepped = std::numeric_limits<int>::min();

    for (int i = 0; i < episodes; i++) {
        auto cur = env.reset(true);
        action_t act = agent->random_action(cur);
        bool terminate = false;
        bool timeout = false;

        int total_steps = 0;
        float total_reward = 0.f;

        while (!terminate && !timeout) {
            auto [next_s, reward, done] = env.step(cur, act);
            agent->observe({
                .s_ = cur,
                .a_ = act,
                .r_ = reward,
                .next_s_ = next_s,
                .done_ = done,
                .timeout_ = timeout
            });
            
            cur = next_s;
            act = agent->sample_action(next_s);
            terminate = done;
            
            total_steps++;
            total_reward += reward;
            
            if(total_steps > 1000) timeout = true;
        }
        eps_exp_sche.step(i);

        agent->flush_buffer();
        
        max_stepped = max_stepped < total_steps ? total_steps : max_stepped;

        if ((i + 1) % 10000 == 0) {
            std::print("EPISODE {}: TOTAL STEPS = {}, TOTAL REWARD = {}\n", i + 1, total_steps, total_reward);
        }
    }
    std::print("epsilon: {}\n", eps_exp_sche.value());
    std::println("MAXIMUM STEPS: {}", max_stepped);
    print_policy_map(agent, env);
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