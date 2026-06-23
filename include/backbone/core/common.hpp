#ifndef _COMMON_HPP_
#define _COMMON_HPP_
#include <bitset>
using state_t = int;
using action_t = int;

using action_mask_t = std::bitset<64>;
using action_mask_internal_t = unsigned long long;

enum class algorithmType {TD, NSTEP, MC};

#endif