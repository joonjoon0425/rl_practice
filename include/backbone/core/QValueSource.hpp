#ifndef _QVALUESOURCE_HPP_
#define _QVALUESOURCE_HPP_

#include <cassert>
#include <memory>
#include <vector>
#include "core/common.hpp"
#include <core/QValueEstimator.hpp>

using state_t = int;
using action_t = int;

class QValueSource {
private:
    std::vector<float> Q_table_;
    int table_size_;
    int state_size_;
    int action_size_;

    std::unique_ptr<QValueEstimator> estimator_;

public:
    struct Proxy {
        QValueSource& source;
        state_t state;
        action_t action;

        Proxy& operator=(float val) {
            if (source.estimator_ == nullptr) {
                source(state, action, 0) = val;
            } else {
                assert(source.estimator_ == nullptr && "estimator should not exist");
            }
            return (*this);
        }

        Proxy& operator+=(float val) {
            if (source.estimator_ == nullptr) {
                source(state, action, 0) += val;
            } else {
                assert(source.estimator_ == nullptr && "estimator should not exist");
            }
            return (*this);
        }

        operator float() const {
            if (source.estimator_) {
                return source.estimator_->estimate(
                    source.Q_table_,
                    source.table_size_,
                    source.state_size_,
                    source.action_size_, state, action);
            } else {
                return source(state, action, 0);
            }
        }
    };

    QValueSource(int state_size, int action_size, float init = 0.0f, int table_size = 1, std::unique_ptr<QValueEstimator> estimator = nullptr);
    float& operator()(state_t state, action_t action, int table);
    Proxy operator()(state_t state, action_t action);
    float operator()(state_t state, action_t action, int table = -1) const;
    
    action_t greedy_action(const state_t& state, const action_mask_t& possible_actions, int table = -1) const;
    action_mask_t greedy_actions(const state_t& state, const action_mask_t& possible_actions, int table = -1) const;

    float max(const state_t& state, const action_mask_t& possible_actions, int table = -1) const;

    int index(const state_t& state, const action_t& action) {
        return state * action_size_ + action;
    }

    int state_size() const {
        return state_size_;
    }

    int action_size() const {
        return action_size_;
    }

    int table_size() const {
        return table_size_;
    }
};

#endif