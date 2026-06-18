#include <gridworld.hpp>
#include <cstdlib>
#include <limits>
#include <sarsa.hpp>

namespace gridworld_2D {
    sarsa_agent::sarsa_agent(int width, int height, int action_size, float epsilon, float alpha, float gamma)
    : Q_table(width * height * action_size, 0.0f),
      width_(width),
      height_(height),
      action_size_(action_size),
      epsilon_(epsilon),
      alpha_(alpha),
      gamma_(gamma) {
        
    }

    action sarsa_agent::get_action(state cur) {
        // get an epsilon-greedy action
        action ret_action;
        float rand_val = static_cast<float>(std::rand()) / RAND_MAX;

        if (rand_val < epsilon_) {
            //explore
            ret_action = random_action(cur);
        } else {
            //exploit
            ret_action = greedy_action(cur);
        }

        return ret_action;
    }

    void sarsa_agent::update(const state& s, const action& a, float r_next, const state& s_next, const action& a_next)  {
        Q_table[idx(s,a)] += alpha_ * (r_next + gamma_ * Q_table[idx(s_next, a_next)] - Q_table[idx(s,a)]);
    }
    
    float& sarsa_agent::epsilon(float val) {
        epsilon_ = val;
        return epsilon_;
    }

    float& sarsa_agent::epsilon() {
        return epsilon_;
    }

    action sarsa_agent::random_action(const state& cur) const {
        auto actions = grid_env::get_possible_actions(width_, height_, cur);
        int rand_idx = std::rand() % actions.size();
        return *std::next(actions.begin(), rand_idx);
    }

    action sarsa_agent::greedy_action(const state& cur) const {
        auto actions = grid_env::get_possible_actions(width_, height_, cur);
        action ret = *actions.begin();
        float max = -std::numeric_limits<float>::infinity();

        for (auto act: actions) {
            if (Q_table[idx(cur, act)] > max) {
                max = Q_table[idx(cur, act)];
                ret = act;
            }
        }

        return ret;
    }
}