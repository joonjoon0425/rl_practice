#include <gridworld.hpp>
#include <cstdlib>
#include <limits>
#include <sarsa.hpp>

namespace gridworld_2D {
    sarsa_agent::sarsa_agent(int width, int height, int action_size, float epsilon, float alpha, float gamma, float init)
    : Q_table(width * height * action_size, init),
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

    void sarsa_agent::update(const sarsa_transition& t)  {
        float next_q = t.done ? 0.f : Q_table[idx(t.s_next, t.a_next)];
        float target = t.reward + gamma_ * next_q;
        Q_table[idx(t.s, t.a)] += alpha_ * (target - Q_table[idx(t.s,t.a)]);
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
        float max = -std::numeric_limits<float>::infinity();
        std::vector<action> max_actions;

        for (auto act: actions) {
            if (Q_table[idx(cur, act)] > max) {
                max = Q_table[idx(cur, act)];
                max_actions.clear();
                max_actions.push_back(act);
            } else if (Q_table[idx(cur, act)] == max) {
                max_actions.push_back(act);
            }
        }

        return max_actions[std::rand() % max_actions.size()];
    }

    int sarsa_agent::max_q(const state& cur) const {
        auto actions = grid_env::get_possible_actions(width_, height_, cur);
        float max = -std::numeric_limits<float>::infinity();

        for (auto act: actions) {
            if (Q_table[idx(cur, act)] > max) {
                max = Q_table[idx(cur, act)];
            }
        }

        return max;
    }
}