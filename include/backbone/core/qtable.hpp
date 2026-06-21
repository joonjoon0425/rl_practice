#ifndef _Q_TABLE_HPP_
#define _Q_TABLE_HPP_

#include <cassert>
#include <vector>

using state_t = int;
using action_t = int;

class QTables {
private:
    std::vector<float> Q_table_;
    int table_num_;
    int state_size_;
    int action_size_;

public:
    QTables(int state_size, int action_size, float init = 0.0f, int table_num = 1)
    : table_num_(table_num),
      state_size_(state_size),
      action_size_(action_size),
      Q_table_(table_num * state_size * action_size, init) {}

    float& operator()(state_t state, action_t action, int table = -1) {
        assert(-1 <= table && table < table_num_ && "out of index");

        if (table != -1) return Q_table_[table * (state_size_ * action_size_) + state * action_size_ + action];
        else return Q_table_[state * action_size_ + action]; // implement qtable estimator here! For now, we use single one.
    }

    float operator()(state_t state, action_t action, int table = -1) const {
        assert(-1 <= table && table < table_num_ && "out of index");

        if (table != -1) return Q_table_[table * (state_size_ * action_size_) + state * action_size_ + action];
        else return Q_table_[state * action_size_ + action]; // implement qtable estimator here! For now, we use single one.
    }
    
};

#endif