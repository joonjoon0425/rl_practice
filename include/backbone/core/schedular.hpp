#ifndef _SCHEDULAR_HPP_
#define _SCHEDULAR_HPP_

#include <algorithm>
#include <functional>

template <typename T>
class schedular {
private:
    using decay_fn = std::function<T(T, int)>;

    T& value_;
    T min_;
    decay_fn decay_;

public:
    schedular(T& value, T min, decay_fn decay)
    : value_(value),
      min_(min),
      decay_(decay) {}

    void step(int episode) {
        value_ = std::max(decay_(value_, episode), min_);
    }

    T value() const {return value_;}

};

#endif