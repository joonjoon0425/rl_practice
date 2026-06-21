#ifndef _UPDATER_HPP_
#define _UPDATER_HPP_

#include <vector>
#include "buffer.hpp"
#include "qtable.hpp"

using state_t = int;
using action_t = int;

class updater {
public:
    virtual ~updater() = default;

    virtual void update(QTables& Q_table, const std::vector<transition>& data, float gamma, float alpha) = 0;
};


#endif