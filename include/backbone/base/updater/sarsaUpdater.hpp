#ifndef _SARSA_UPDATER_HPP_
#define _SARSA_UPDATER_HPP_

#include "core/updater.hpp"

class sarsaUpdater : public updater {
public:
    sarsaUpdater();

    void update(QTables& Q_table, const std::vector<transition>& data, float gamma, float alpha) override;
};

#endif