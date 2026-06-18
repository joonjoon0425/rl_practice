#ifndef _GRIDWORLD_HPP_
#define _GRIDWORLD_HPP_

#include <env.hpp>
#include <point.hpp>
#include <set>

namespace gridworld_2D{
    constexpr size_t ACTION_NUM = 4;
    using state = point;

    enum class action: int {up, down, left, right};

    class grid_env : public environment<state, action> {
        state cur_;
        state start_;
        state goal_;
        state trap_;

        int width_;
        int height_;
        
    public:
        grid_env(int width, int height, state start, state goal, state trap);
        void reset(bool random = false);
        std::tuple<state, float, bool> step(action);

        static std::set<action> get_possible_actions(int, int, state);
        
        state cur() const {return cur_;}
        state trap() const {return trap_;}
        state goal() const {return goal_;}

        int width() const {return width_;}
        int height() const {return height_;}
    };
}

#endif