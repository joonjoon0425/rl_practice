#ifndef _MONTE_CARLO_UPDATER_HPP_
#define _MONTE_CARLO_UPDATER_HPP_

#include <core/updater.hpp>

class onPolicyMonteCarloUpdater : public updater {
private:
    bool every_visit_;
    std::vector<int> N_;

public:
    onPolicyMonteCarloUpdater(bool every_visit, int state_size, int action_size)
    : every_visit_(every_visit),
      N_(state_size * action_size, 0)
      {}

    void update(QTables &Q_table, const std::vector<transition> &data, float gamma, float alpha) override;
};

class offPolicyMonteCarloUpdater : public updater {
private:
    std::vector<float> C_;

public:
    offPolicyMonteCarloUpdater(int state_size, int action_size)
    : C_(state_size * action_size, 0)
      {}
    
    void update(QTables &Q_table, const std::vector<transition> &data, float gamma, float alpha) override;
};

#endif