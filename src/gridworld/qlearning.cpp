#include "gridworld.hpp"
#include <qlearning.hpp>
#include <cstdlib>

namespace gridworld_2D {
    grid_q_agent::grid_q_agent(int width, int height, int action_size, float epsilon, float alpha, float gamma, float init)
    : width_(width),
      height_(height),
      epsilon_(epsilon),
      q_agent(width * height, action_size, alpha, gamma, init) {}


    grid_env::action_t grid_q_agent::get_action(const grid_env::state_t& cur) const {
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