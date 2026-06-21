#include <gridworld.hpp>
#include <cstdlib>

namespace gridworld {
    grid2D::grid2D(int width, int height, envState_t start, envState_t goal, envState_t trap)
    : width_(width),
      height_(height),
      start_(start),
      goal_(goal),
      trap_(trap),
      environment(width * height, 4)
    {}

    state_t grid2D::reset(bool random) {
        if (random) return std::rand() % state_size_;

        return state_to_index(start_);
    }
    
    std::tuple<state_t, float, bool, action_mask_t> grid2D::step(const state_t& state, const action_t& action) {
        // assumes that only possible actions are received as input
        envState_t state_ = index_to_state(state);
        envAction_t act_ = index_to_action(action);
        switch(act_) {
            case envAction_t::UP: state_.y_++; break;
            case envAction_t::DOWN: state_.y_--; break;
            case envAction_t::LEFT: state_.x_--; break;
            case envAction_t::RIGHT: state_.x_++; break;
        };

        float reward = -0.2f;
        bool done = false;
        if (state_ == goal_) {
            reward = 15.0f;
            done = true;
        } else if (state_ == trap_) {
            reward = -10.0f;
            done = true;
        }

        int idx = state_to_index(state_);

        return {idx, reward, done, get_possible_actions(idx)};
    }
}