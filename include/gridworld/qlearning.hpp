#ifndef _Q_LEARNING_HPP_
#define _Q_LEARNING_HPP_

#include <gridworld.hpp>
#include <q_agent.hpp>

#include <cassert>

namespace gridworld_2D {
    class grid_q_agent : public q_agent<grid_env> {
        int width_;
        int height_;
        float epsilon_;

    public:
        grid_q_agent(int width, int height, int action_size, float epsilon, float alpha, float gamma, float init);
        grid_env::action_t get_action(const grid_env::state_t&) const override;
        
        float& epsilon(float val) {return epsilon_ = val;};
        float& epsilon() {return epsilon_;}
        
        int idx(const grid_env::state_t& state, const grid_env::action_t& act) const override {
            int act_int = static_cast<int>(act);
            assert(state.x_ >= 0 && state.x_ < width_  && "state.x가 범위를 벗어남!");
            assert(state.y_ >= 0 && state.y_ < height_ && "state.y가 범위를 벗어남!");
            assert(act_int >= 0 && act_int < action_size_ && "action 값이 action_size보다 크거나 음수임!");
            return (state.y_ * width_ + state.x_) * action_size_ + act_int;
        }
    };
}
#endif