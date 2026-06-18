#include <cstdlib>
#include <gridworld.hpp>

namespace gridworld_2D {
    grid_env::grid_env(int width, int height, state start, state goal, state trap)
    : width_(width), height_(height),
      start_(start), goal_(goal), trap_(trap) {}

    void grid_env::reset(bool random) {
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
    }

    std::tuple<state, float, bool> grid_env::step(action act) {
        // assumes that only possible actions are received as input
        switch(act) {
            case action::up: cur_.y++; break;
            case action::down: cur_.y--; break;
            case action::left: cur_.x--; break;
            case action::right: cur_.x++; break;
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

    std::set<action> grid_env::get_possible_actions(int width_, int height_, state cur_) {
        std::set<action> possible_actions({action::up, action::down, action::left, action::right});

        if (cur_.x == width_ - 1) possible_actions.erase(action::right);
        else if (cur_.x == 0) possible_actions.erase(action::left);

        if (cur_.y == height_ - 1) possible_actions.erase(action::up);
        else if (cur_.y == 0) possible_actions.erase(action::down);

        return possible_actions;
    }
};