#include "base/buffer/nStepBuffer.hpp"
#include <iterator>

nStepBuffer::nStepBuffer(int steps) : steps_(steps) {}

std::vector<transition> nStepBuffer::pop() {
    auto ret_data = std::vector<transition>(std::make_move_iterator(data_.begin()), std::make_move_iterator(data_.begin() + steps_));
    data_.pop_front();
    return ret_data;
}

bool nStepBuffer::ready() const {
    if (data_.size() >= steps_) {
        //std::cout << "DEBUG: READY FOR UPDATE\n";
        return true;
    }
    return false;
}