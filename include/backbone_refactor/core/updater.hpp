#ifndef _UPDATER_HPP_
#define _UPDATER_HPP_

#include <vector>
#include "buffer.hpp"

template <typename Env>
class updater {
protected:
    Env& env_;
public:
    virtual ~updater() = default;

    updater(Env& env) : env_(env) {}
    virtual void update(std::vector<float>& Q_table, const std::vector<transition<Env>>& data, float gamma, float alpha) = 0;
};


#endif