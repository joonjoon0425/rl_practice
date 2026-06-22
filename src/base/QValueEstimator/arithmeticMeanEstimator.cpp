#include <base/QValueEstimator/arithmeticMeanEstimator.hpp>

float arithmeticMeanEstimator::estimate(
    const std::vector<float>& Q_table,
    int table_num, int state_size, int action_size,
    int state, int action
) const
{
    float avg = 0.0f;
    int offset = state * action_size + action;
    for (int t = 0; t < table_num; t++) {
        avg += Q_table[t * state_size * action_size + offset];
    }

    return avg / table_num;
}