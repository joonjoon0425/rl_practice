#include "core/schedulable.hpp"
#include "core/schedular.hpp"
#include <cassert>
#include <gridworld.hpp>
#include <limits>
#include <memory>
#include <register.hpp>
#include <print>

void print_policy_map(std::shared_ptr<agent<gridworld::env_grid2D>> agent, const gridworld::env_grid2D& env);

int main() {
    int w = 15;
    int h = 15;
    float init = 0.0f;

    gridworld::env_grid2D env(w, h, {0, 0}, {3, 5}, {3, 3});
    // SARSA
    auto agent = create_agent(env, algo_type::TD, policy_type::ON_POLICY, 1.0);
    auto ptr = std::dynamic_pointer_cast<epsilon_schedulable>(agent->behavior_policy());
    assert(ptr != nullptr && "dynamic cast failure");

    schedular<float> eps_exp_sche((ptr->epsilon()), 0.00f,
        [](float val, int _) {
            return val * 0.999954f;
        }
    );

    int episodes = 1000000;

    int max_stepped = std::numeric_limits<int>::min();

    for (int i = 0; i < episodes; i++) {
        auto cur = env.reset(true);
        auto act = agent->sample_action(cur);
        bool terminate = false;

        int total_steps = 0;
        float total_reward = 0.f;

        while (!terminate) {
            auto [next_s, reward, done] = env.step(cur, act);
            auto next_a = agent->sample_action(next_s);

            agent->observe({cur, act, reward, next_s, done, next_a});
            
            act = next_a;
            

            cur = next_s;
            terminate = done;
            
            total_steps++;
            total_reward += reward;
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

void print_policy_map(std::shared_ptr<agent<gridworld::env_grid2D>> agent, const gridworld::env_grid2D& env) {
    const char* symbol[] = {"↑", "↓", "←", "→"};
    gridworld::env_grid2D::state_t p{0, 0};
    
    for (int y = env.height() - 1; y >= 0; y--) {
        for (int x = 0; x < env.width(); x++) {
            p.x_ = x;
            p.y_ = y;
            
            if (p == env.trap()) {
                std::print("T\t");
            } else if (p == env.goal()) {
                std::print("G\t");
            } else {
                std::print("{}\t", symbol[static_cast<int>(agent->greedy_action(p))], agent->max_q(p));
            }
        }
        std::print("\n");
    }
}