#ifndef _SCHEDULABLE_HPP_
#define _SCHEDULABLE_HPP_

// interfaces

class epsilon_schedulable {
public:
    virtual float& epsilon() = 0;
    virtual float epsilon() const = 0;

    virtual void epsilon(float) = 0;
};

#endif