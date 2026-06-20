#ifndef _GRIDWORLD_HPP_
#define _GRIDWORLD_HPP_

#include "core/env.hpp"

namespace gridworld {
    struct point2D {
        int x_;
        int y_;

        bool operator==(const point2D& point) const {
            return x_ == point.x_ && y_ == point.y_;
        }
    };

    enum class direction2D : int {UP, DOWN, LEFT, RIGHT};

    class env_grid2D : public environment<point2D, direction2D, env_grid2D> {
    public:
        env_grid2D(int width, int height, state_t start, state_t goal, state_t trap);

        state_t reset(bool random) override;
        std::tuple<state_t, float, bool> step(const state_t&, const action_t&) override;

    private:
        int width_;
        int height_;
        
        state_t start_;
        state_t goal_;
        state_t trap_;

    public:
        int width() const {return width_;}
        int height() const {return height_;}

        state_t trap() const {return trap_;}
        state_t goal() const {return goal_;}
        state_t start() const {return start_;}

        int impl_state_to_index(const state_t& state) const {
            return state.y_ * width_ + state.x_;
        }

        int impl_action_to_index(const action_t& action) const {
            return static_cast<int>(action);
        }

        std::set<action_t> impl_get_possible_actions(const state_t& state) const {
            std::set<action_t> actions{action_t::UP, action_t::DOWN, action_t::LEFT, action_t::RIGHT};
            if (state.x_ == width_ - 1) actions.erase(action_t::RIGHT);
            else if (state.x_ == 0) actions.erase(action_t::LEFT);
            
            if (state.y_ == height_ - 1) actions.erase(action_t::UP);
            else if (state.y_ == 0) actions.erase(action_t::DOWN);

            return actions;
        }
    };
}

#endif