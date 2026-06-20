#ifndef _POLICY_HPP_
#define _POLICY_HPP_

#include <vector>

template <typename Env>
class policy {
protected:
    Env& env_;
public:
    policy(Env& env) : env_(env) {}
    virtual ~policy() = default;

    virtual Env::action_t get_action(const std::vector<float>& Q_table_, const Env::state_t& state) const = 0;
    virtual float get_prob(const std::vector<float>& Q_table_, const Env::state_t& state, const Env::action_t& action) const = 0;
};


#endif