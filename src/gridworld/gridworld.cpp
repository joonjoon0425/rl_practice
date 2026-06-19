#include <cstdlib>
#include <gridworld.hpp>

namespace gridworld_2D {
    grid_env::grid_env(int width, int height, grid_state start, grid_state goal, grid_state trap)
    : width_(width), height_(height),
      start_(start), goal_(goal), trap_(trap),
      cur_(start) {}

    void grid_env::reset(bool random) {
        if (random) {
            do {
                int rand = std::rand();
                int x = rand % width_;
                int y = rand % height_;
                cur_ = {x, y, width_, height_};
            } while (cur_ == trap_ || cur_ == goal_);
        } else {
            cur_ = start_;
        }
    }

    std::tuple<grid_state, float, bool> grid_env::step(grid_action act) {
        // assumes that only possible actions are received as input
        switch(act) {
            case grid_action::up: cur_.y_++; break;
            case grid_action::down: cur_.y_--; break;
            case grid_action::left: cur_.x_--; break;
            case grid_action::right: cur_.x_++; break;
        };

        float reward = -1.0f;
        bool done = false;
        if (cur_ == goal_) {
            reward = 10.0f;
            done = true;
        } else if (cur_ == trap_) {
            reward = -8.0f;
            done = true;
        }

        return {cur_, reward, done};
    }
};