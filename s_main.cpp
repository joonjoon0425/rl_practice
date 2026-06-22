#include "core/schedulable.hpp"
#include "core/schedular.hpp"
#include <cassert>
#include <gridworld.hpp>
#include <limits>
#include <memory>
#include <register.hpp>
#include <print>

void print_policy_map(std::shared_ptr<agent> agent, const gridworld::grid2D& env);

int main() {
    int w = 15;
    int h = 15;
    float init = 0.0f;

    gridworld::grid2D env(w, h, {0, 0}, {14, 14}, {13, 13});
    // SARSA
    auto agent = create_agent(env.state_size(), env.action_size(), [&](const state_t& s){return env.get_possible_actions(s);}, algoType::sarsa, 1.0);
    auto ptr = std::dynamic_pointer_cast<epsilonSchedulable>(agent->behavior_policy());
    assert(ptr != nullptr && "dynamic cast failure");

    schedular<float> eps_exp_sche((ptr->epsilon()),
        [](float val, int _) {
            return val * 0.999954f;
        }
    );

    int episodes = 5000000;

    int max_stepped = std::numeric_limits<int>::min();

    for (int i = 0; i < episodes; i++) {
        auto cur = env.reset(true);
        auto act = agent->sample_action(cur);
        bool terminate = false;
        bool timeout = false;

        int total_steps = 0;
        float total_reward = 0.f;

        while (!terminate && !timeout) {
            auto [next_s, reward, done] = env.step(cur, act);
            auto next_a = agent->sample_action(next_s);

            agent->observe({
                    .s_ = cur,
                    .a_ = act,
                    .r_ = reward,
                    .next_s_ =  next_s,
                    .next_a_ = next_a,
                    .done_ = done,
                    .timeout_ = timeout
                });
            
            act = next_a;
            

            cur = next_s;
            terminate = done;
            
            total_steps++;
            total_reward += reward;
            
            // if(total_steps > 5000) timeout = true;
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

void print_policy_map(std::shared_ptr<agent> agent, const gridworld::grid2D& env) {
    const char* symbol[] = {"↑", "↓", "←", "→"};
    gridworld::grid2D::envState_t p{0, 0};
    
    for (int y = env.height() - 1; y >= 0; y--) {
        for (int x = 0; x < env.width(); x++) {
            p.x_ = x;
            p.y_ = y;
            
            if (p == env.trap()) {
                std::print("T\t");
            } else if (p == env.goal()) {
                std::print("G\t");
            } else {
                std::print("{}\t",
                    symbol[static_cast<int>(agent->greedy_action(env.state_to_index(p)))],
                    agent->max_q(env.state_to_index(p)));
            }
        }
        std::print("\n");
    }
}