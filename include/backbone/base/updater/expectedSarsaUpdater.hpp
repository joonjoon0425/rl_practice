#ifndef _EXPECTED_SARSA_UPDATER
#define _EXPECTED_SARSA_UPDATER

#include <core/updater.hpp>
#include <core/policy.hpp>
#include <memory>

class expectedSarsaUpdater : public updater{
private:
    std::shared_ptr<policy> target_policy_;

public:
    expectedSarsaUpdater(std::shared_ptr<policy> target_policy) : target_policy_(target_policy) {}
    
    void update(QTables& Q_table, const std::vector<transition>& data, float gamma, float alpha) override;
};

#endif