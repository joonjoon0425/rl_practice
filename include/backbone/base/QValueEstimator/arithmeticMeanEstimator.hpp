#ifndef _ARITHMETIC_MEAN_ESTIMATOR_HPP_
#define _ARITHMETIC_MEAN_ESTIMATOR_HPP_

#include <core/QValueEstimator.hpp>

class arithmeticMeanEstimator : public QValueEstimator {
public:
    float estimate(const std::vector<float>& Q_table, int table_num, int state_size, int action_size, int state, int action) const override;
};

#endif