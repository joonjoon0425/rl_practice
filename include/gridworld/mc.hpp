#include <gridworld.hpp>
#include <monte_carlo_agent.hpp>
#include <cassert>

namespace gridworld_2D {
    class grid_mc_agent : public monte_carlo_agent<grid_env> {
        int width_;
        int height_;

    public:
        grid_mc_agent(int width_, int height_, int action_size, float epsilon, float gamma, bool every_visit);
        grid_env::action_t get_action(const grid_env::state_t&) const override;

        int idx(const grid_env::state_t& state, const grid_env::action_t& act) const override {
            int act_int = static_cast<int>(act);
            assert(state.x_ >= 0 && state.x_ < width_  && "state.x가 범위를 벗어남!");
            assert(state.y_ >= 0 && state.y_ < height_ && "state.y가 범위를 벗어남!");
            assert(act_int >= 0 && act_int < action_size_ && "action 값이 action_size보다 크거나 음수임!");
            return (state.y_ * width_ + state.x_) * action_size_ + act_int;
        }
    };
};