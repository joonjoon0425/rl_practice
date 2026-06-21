#ifndef _SCHEDULAR_HPP_
#define _SCHEDULAR_HPP_

#include <functional>

template <typename T>
class schedular {
private:
    using decay_fn = std::function<T(T, int)>;

    T& value_;
    decay_fn decay_;

public:
    schedular(T& value, decay_fn decay)
    : value_(value),
      decay_(decay) {}

    void step(int episode) {
        value_ = decay_(value_, episode);
    }

    T value() const {return value_;}

};

#endif