#include "point.hpp"
#include "qlearning.hpp"
#include <gridworld.hpp>
#include <sarsa.hpp>
#include <print>

void print_policy_map(const gridworld_2D::q_learning_agent& agent, const gridworld_2D::grid_env& env) {
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
                std::print("{}\t", /*symbol[static_cast<int>(agent.greedy_action(p))]*/ agent.max_q(p));
            }
        }
        std::print("\n");
    }
}

int main() {
    int width = 10;
    int height = 10;
    float init = 0.f;
    gridworld_2D::grid_env env(width, height, {0, 0}, {3, 4}, {3, 3});
    gridworld_2D::q_learning_agent q_agent(width, height, gridworld_2D::ACTION_NUM, 1.0, 0.4, 0.9, init);

    int episodes = 1000000;

    // Q-learning agent training
    std::print("Q_LEARNING AGENT TRAINING\n");
    for (int i = 0; i < episodes; i++) {
        env.reset(true);
        auto cur = env.cur();
        bool terminate = false;

        float total_reward = 0.f;
        int total_steps = 0;

        while (!terminate) {
            auto act = q_agent.get_action(cur);
            auto [s_next, reward, done] = env.step(act);
            q_agent.update({cur, act, reward, s_next, done});
            cur = s_next;
            terminate = done;

            // for check
            total_reward += reward;
            total_steps++;
        }
        q_agent.epsilon() *= 0.9996;
        if (i % 10000 == 0) std::print("Episode {}: total reward: {}, total steps: {}\n", i, total_reward, total_steps);
    }

    // print policy map
    print_policy_map(q_agent, env);
    
}