#include <base/buffer/montecarlobuffer.hpp>

bool monteCarloBuffer::ready() const {
    if (data_.empty()) return false;
    return data_.back().done_;
}

std::vector<transition> monteCarloBuffer::pop() {
    std::vector<transition> ret(std::make_move_iterator(data_.begin()), std::make_move_iterator(data_.end()));
    data_.clear();
    return ret;
}