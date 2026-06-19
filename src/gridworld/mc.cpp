#include <mc.hpp>

namespace gridworld_2D {
    grid_mc_agent::grid_mc_agent(int width, int height, int action_size, float epsilon, float gamma, bool every_visit)
    : monte_carlo_agent(width * height, action_size, epsilon, gamma, every_visit),
      width_(width),
      height_(height) {}
    
    grid_env::action_t grid_mc_agent::get_action(const grid_env::state_t& state) const {
        grid_env::action_t ret_action;
        float rand_val = static_cast<float>(std::rand()) / RAND_MAX;

        if (rand_val < epsilon_) {
            //explore
            ret_action = random_action(state);
        } else {
            //exploit
            ret_action = greedy_action(state);
        }

        return ret_action;
    }
};