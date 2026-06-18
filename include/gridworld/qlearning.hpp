#ifndef _Q_LEARNING_HPP_
#define _Q_LEARNING_HPP_

#include <gridworld.hpp>
#include <agent.hpp>

#include <vector>
#include <cassert>

namespace gridworld_2D {
    struct q_learning_transition {
        state s;
        action a;
        float reward;
        state s_next;
        bool done;
    };
    
    class q_learning_agent : public agent<state, action, q_learning_transition> {
        std::vector<float> Q_table;
        int width_;
        int height_;
        int action_size_;

        float epsilon_;
        float alpha_;
        float gamma_;
    public:
        q_learning_agent(int width, int height, int action_size, float epsilon, float alpha, float gamma, float init);
        gridworld_2D::action get_action(state) override;
        void update(const q_learning_transition&) override;
        action greedy_action(const state&) const;
        int max_q(const state&) const;
        
        float& epsilon(float val) {return epsilon_ = val;};
        float& epsilon() {return epsilon_;}

    private:
        action random_action(const state&) const;
        
        int idx(const state& state, const action& act) const {
            int act_int = static_cast<int>(act);
            assert(state.x >= 0 && state.x < width_  && "state.x가 범위를 벗어남!");
            assert(state.y >= 0 && state.y < height_ && "state.y가 범위를 벗어남!");
            assert(act_int >= 0 && act_int < action_size_ && "action 값이 action_size보다 크거나 음수임!");
            return (state.y * width_ + state.x) * action_size_ + act_int;
        }
    };
}
#endif