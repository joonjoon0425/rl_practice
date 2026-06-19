#include "point.hpp"
#include "mc.hpp"
#include <gridworld.hpp>
#include <print>

void print_policy_map(const gridworld_2D::grid_mc_agent& agent, const gridworld_2D::grid_env& env) {
    const char* symbol[] = {"↑", "↓", "←", "→"};
    gridworld_2D::point p(0, 0, env.width(), env.height());
    
    for (int y = env.height() - 1; y >= 0; y--) {
        for (int x = 0; x < env.width(); x++) {
            p.x_ = x;
            p.y_ = y;
            
            if (p == env.trap()) {
                std::print("T\t");
            } else if (p == env.goal()) {
                std::print("G\t");
            } else {
                std::print("{}\t", symbol[static_cast<int>(agent.greedy_action(p))]/*agent.max_q(p)*/);
            }
        }
        std::print("\n");
    }
}

int main() {
    int width = 15;
    int height = 15;
    float init = 0.f;
    gridworld_2D::grid_env env(width, height, {0, 0, width, height}, {10, 4, width, height}, {10, 3, width, height});
    gridworld_2D::grid_mc_agent mc_agent(width, height, gridworld_2D::ACTION_NUM, 1.0, 0.9, false);

    int episodes = 100000;
    std::vector<gridworld_2D::grid_env::state_t> states;
    std::vector<gridworld_2D::grid_env::action_t> actions;
    std::vector<float> rewards;

    // mc agent training
    std::print("MONTE CARLO (ON-POLICY) AGENT TRAINING\n");
    for (int i = 0; i < episodes; i++) {
        env.reset(true);
        auto cur = env.cur();
        auto act = mc_agent.random_action(cur);
        bool terminate = false;

        float total_reward = 0.f;
        int total_steps = 0;

        while (!terminate) {
            states.push_back(cur);
            actions.push_back(act);
            auto [s_next, reward, done] = env.step(act);    
            cur = s_next;
            act = mc_agent.get_action(cur);
            rewards.push_back(reward);
            terminate = done;

            // for check
            total_reward += reward;
            total_steps++;

            if (total_steps > 2000) {
                break;
            }
        }
        mc_agent.epsilon() *= 0.9999;
        if (i % 100 == 0) std::print("Episode {}: total reward: {}, total steps: {}\n", i, total_reward, total_steps);
        
        mc_agent.update({states, actions, rewards});
        
        states.clear();
        actions.clear();
        rewards.clear();
    }

    // print policy map
    print_policy_map(mc_agent, env);
    
}