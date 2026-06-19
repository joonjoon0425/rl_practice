#ifndef _GRIDWORLD_HPP_
#define _GRIDWORLD_HPP_

#include <env.hpp>
#include <point.hpp>
#include <set>

namespace gridworld_2D{
    constexpr size_t ACTION_NUM = 4;
    using grid_state = point;

    enum class grid_action: int {up, down, left, right};

    class grid_env : public environment<grid_state, grid_action, grid_env> {
        grid_state cur_;
        grid_state start_;
        grid_state goal_;
        grid_state trap_;

        int width_;
        int height_;
        
    public:
        grid_env(int width, int height, grid_state start, grid_state goal, grid_state trap);
        void reset(bool random = false);
        std::tuple<grid_state, float, bool> step(grid_action);
        
        grid_state cur() const {return cur_;}
        grid_state trap() const {return trap_;}
        grid_state goal() const {return goal_;}

        int width() const {return width_;}
        int height() const {return height_;}

        static std::set<grid_env::action_t> impl_get_possible_actions(const grid_env::state_t& state) {
            std::set<grid_env::action_t> actions{grid_action::up, grid_action::down, grid_action::left, grid_action::right};
            if (state.x_ == state.max_x_ - 1) actions.erase(grid_action::right);
            else if (state.x_ == 0) actions.erase(grid_action::left);
            
            if (state.y_ == state.max_y_ - 1) actions.erase(grid_action::up);
            else if (state.y_ == 0) actions.erase(grid_action::down);

            return actions;
        }
    };
}

#endif