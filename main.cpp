#include "point.hpp"
#include <gridworld.hpp>
#include <sarsa.hpp>
#include <print>

void print_policy_map(const gridworld_2D::sarsa_agent& agent, const gridworld_2D::grid_env& env) {
    const char* symbol[] = {"↑", "↓", "←", "→"};
    gridworld_2D::point p;
    
    for (int y = env.height() - 1; y >= 0; y--) {
        for (int x = 0; x < env.width(); x++) {
            p.x = x;
            p.y = y;
            
            if (p == env.trap()) {
                std::print("T\t");
            } else if (p == env.goal()) {
                std::print("G\t");
            } else {
                std::print("{}\t", symbol[static_cast<int>(agent.greedy_action(p))]);
            }
        }
        std::print("\n");
    }
}

int main() {
    int width = 5;
    int height = 5;
    gridworld_2D::grid_env env(width, height, {0, 0}, {4, 4}, {2, 2});
    gridworld_2D::sarsa_agent agent(width, height, gridworld_2D::ACTION_NUM, 0.1, 0.3, 0.9);

    int episodes = 2000;

    for (int i = 0; i < episodes; i++) {
        env.reset(true);
        auto cur = env.cur();
        auto act = agent.get_action(cur);
        bool terminate = false;

        float total_reward = 0.0f;
        int total_steps = 0;

        while (!terminate) {
            auto [next, reward, done] = env.step(act);
            terminate = done;
            auto act_next = agent.get_action(next);
            agent.update(cur, act, reward, next, act_next);
            act = act_next;
            cur = next;

            // for checking if the agent is learning well
            total_steps++;
            total_reward += reward;
        }
        agent.epsilon() *= 0.99f;

        // for checking if the agent is learning well
        if (i % 50 == 0) std::print("Episode {}: total reward: {}, total steps: {}\n", i, total_reward, total_steps);
        
    }

    // print policy nap
    print_policy_map(agent, env);
}