#ifndef _QVALUEESTIMATOR_HPP_
#define _QVALUEESTIMATOR_HPP_

#include <vector>

class QValueEstimator {
public:
    virtual ~QValueEstimator() = default;
    virtual float estimate(const std::vector<float>& Q_table, int table_num, int state_size, int action_size, int state, int action) const = 0;
};

#endif