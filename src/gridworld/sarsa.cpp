#include <gridworld.hpp>
#include <sarsa.hpp>

namespace gridworld_2D {
    grid_sarsa_agent::grid_sarsa_agent(int width, int height, int action_size, float epsilon, float alpha, float gamma, float init)
    : width_(width),
      height_(height),
      epsilon_(epsilon),
      sarsa_agent(width * height, action_size, alpha, gamma, init) {}

    grid_env::action_t grid_sarsa_agent::get_action(const grid_env::state_t& cur) const {
        grid_env::action_t ret_action;
        float rand_val = static_cast<float>(std::rand()) / RAND_MAX;

        if (rand_val < epsilon_) {
            //explore
            ret_action = random_action(cur);
        } else {
            //exploit
            ret_action = greedy_action(cur);
        }

        return ret_action;
    }
}