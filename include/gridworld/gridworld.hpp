#ifndef _GRIDWORLD_HPP_
#define _GRIDWORLD_HPP_

#include "core/env.hpp"
#include <algorithm>

namespace gridworld {
    struct point2D {
        int x_;
        int y_;

        bool operator==(const point2D& point) const {
            return x_ == point.x_ && y_ == point.y_;
        }
    };

    enum class direction2D : int {UP, DOWN, LEFT, RIGHT};

    class grid2D : public environment<point2D, direction2D, grid2D> {
    public:
        grid2D(int width, int height, envState_t start, envState_t goal, envState_t trap);

        state_t reset(bool random) override;
        std::tuple<state_t, float, bool, std::vector<bool>> step(const state_t&, const action_t&) override;

    private:
        int width_;
        int height_;
        
        envState_t start_;
        envState_t goal_;
        envState_t trap_;

    public:
        int width() const {return width_;}
        int height() const {return height_;}

        envState_t trap() const {return trap_;}
        envState_t goal() const {return goal_;}
        envState_t start() const {return start_;}

        int impl_state_to_index(const envState_t& state) const {
            return state.y_ * width_ + state.x_;
        }

        int impl_action_to_index(const envAction_t& action) const {
            return static_cast<int>(action);
        }

        envState_t impl_index_to_state(int index) const {
            int x_ = index % width_;
            int y_ = index / width_;

            return {x_, y_};
        }

        envAction_t impl_index_to_action(int index) const {
            return static_cast<envAction_t>(index);
        }

        std::vector<bool> impl_get_possible_actions(const state_t& state_) const {
            std::vector<bool> possible_actions(action_size_, true);
            envState_t state = index_to_state(state_);
            if (state.x_ == width_ - 1) possible_actions[action_to_index(envAction_t::RIGHT)] = false;
            else if (state.x_ == 0) possible_actions[action_to_index(envAction_t::LEFT)] = false;
            
            if (state.y_ == height_ - 1) possible_actions[action_to_index(envAction_t::UP)] = false;
            else if (state.y_ == 0) possible_actions[action_to_index(envAction_t::DOWN)] = false;

            return possible_actions;
        }
    };
}

#endif