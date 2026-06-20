#include <gridworld.hpp>
#include <cstdlib>

namespace gridworld {
    env_grid2D::env_grid2D(int width, int height, state_t start, state_t goal, state_t trap)
    : width_(width),
      height_(height),
      start_(start),
      goal_(goal),
      trap_(trap),
      environment(width * height, 4)
    {}

    env_grid2D::state_t env_grid2D::reset(bool random) {
        state_t cur_{0,0};
        if (random) {
            do {
                int rand = std::rand();
                int x = rand % width_;
                int y = rand % height_;
                cur_ = {x, y};
            } while (cur_ == trap_ || cur_ == goal_);
        } else {
            cur_ = start_;
        }

        return cur_;
    }
    
    std::tuple<env_grid2D::state_t, float, bool> env_grid2D::step(const state_t& state, const action_t& action) {
        // assumes that only possible actions are received as input
        state_t cur_ = state;
        switch(action) {
            case action_t::UP: cur_.y_++; break;
            case action_t::DOWN: cur_.y_--; break;
            case action_t::LEFT: cur_.x_--; break;
            case action_t::RIGHT: cur_.x_++; break;
        };

        float reward = -1.0f;
        bool done = false;
        if (cur_ == goal_) {
            reward = 10.0f;
            done = true;
        } else if (cur_ == trap_) {
            reward = -10.0f;
            done = true;
        }

        return {cur_, reward, done};
    }
}