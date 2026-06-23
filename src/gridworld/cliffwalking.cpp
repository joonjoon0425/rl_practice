#include <cliffwalking.hpp>

cliffWalking::cliff2D::cliff2D(int width, int height, envState_t start, envState_t goal, std::set<envState_t> traps) 
: width_(width),
  height_(height),
  start_(start),
  goal_(goal),
  traps_(traps),
  environment(width * height, 4)
{

}

state_t cliffWalking::cliff2D::reset(bool random) {
    if (random) {
        int ret = 0;
        do {
            ret = std::rand() % state_size_;
        } while (traps_.find(index_to_state(ret)) != traps_.end() || index_to_state(ret) == start_);
        return ret;
    }

    return state_to_index(start_);
}

std::tuple<state_t, float, bool> cliffWalking::cliff2D::step(const state_t& state, const action_t& action) {
    // assumes that only possible actions are received as input
        envState_t state_ = index_to_state(state);
        envAction_t act_ = index_to_action(action);
        switch(act_) {
            case envAction_t::UP: state_.y_++; break;
            case envAction_t::DOWN: state_.y_--; break;
            case envAction_t::LEFT: state_.x_--; break;
            case envAction_t::RIGHT: state_.x_++; break;
        };

        float reward = -2.f;
        bool done = false;
        if (state_ == goal_) {
            reward = 1000.0f;
            done = true;
        } else if (traps_.find((state_)) != traps_.end()) {
            reward = -100.0f;
            done = true;
        }

        int idx = state_to_index(state_);

        return {idx, reward, done};
}