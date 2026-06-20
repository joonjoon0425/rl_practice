#include <gridworld.hpp>
#include <register.hpp>
#include <print>

void print_policy_map(std::shared_ptr<agent<gridworld::env_grid2D>> agent, const gridworld::env_grid2D& env);

int main() {
    int w = 10;
    int h = 10;
    float init = 0.0f;

    gridworld::env_grid2D env(w, h, {0, 0}, {8, 8}, {7, 8});
    // SARSA
    auto agent = create_agent(env, algo_type::TD, policy_type::ON_POLICY, 0.1, 0.9, 0.5, 20.0f);
    // QLEARN
    //auto agent = create_agent(env, algo_type::TD, policy_type::OFF_POLICY, 0.1);
    
    int episodes = 100000;

    for (int i = 0; i < episodes; i++) {
        auto cur = env.reset(false);
        auto act = agent->sample_action(cur);
        bool terminate = false;

        int total_steps = 0;
        float total_reward = 0.f;

        while (!terminate) {
//-----------------------------------------
            auto [next_s, reward, done] = env.step(cur, act);
            auto next_a = agent->sample_action(next_s);

            agent->observe({cur, act, reward, next_s, done, next_a});
            
            act = next_a;
            
//-----------------------------------------
            // auto a = agent->sample_action(cur);
            // auto [next_s, reward, done] = env.step(cur, a);
            // agent->observe({cur, a, reward, next_s, done});
//-----------------------------------------
            cur = next_s;
            terminate = done;
            
            total_steps++;
            total_reward += reward;
        }

        agent->flush_buffer();
        

        if ((i + 1) % 100 == 0) {
            std::print("EPISODE {}: TOTAL STEPS = {}, TOTAL REWARD = {}\n", i + 1, total_steps, total_reward);
        }
    }

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
                std::print("{}\t", symbol[static_cast<int>(agent->predict_action(p))]/*agent.max_q(p)*/);
            }
        }
        std::print("\n");
    }
}