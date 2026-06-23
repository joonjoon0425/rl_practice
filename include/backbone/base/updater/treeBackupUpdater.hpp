#ifndef _TREE_BACKUP_UPDATER_HPP_
#define _TREE_BACKUP_UPDATER_HPP_

#include "core/policy.hpp"
#include <core/updater.hpp>
#include <memory>

class treeBackupUpdater : public updater {
private:
    std::shared_ptr<policy> target_policy_;

public:
    treeBackupUpdater(std::shared_ptr<policy> target_policy) : target_policy_(target_policy) {}
    void update(QValueSource &Q_table, const std::vector<transition> &data, float gamma, float alpha) override;
};

#endif