#include <core/policy.hpp>

action_t policy::random_action(const state_t& state, const action_mask_t& possible_actions) const {
    action_mask_internal_t mask = possible_actions.to_ullong();
    assert(mask > 0 && "no actions");
    int size = possible_actions.count();

    int rand = std::rand() % size;

    for (int i = 0; i < rand; i++) mask &= (mask - 1);

    assert(mask != 0 && "mask became zero unexpectedly");
    return __builtin_ctzll(mask);
}