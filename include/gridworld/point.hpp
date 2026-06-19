#ifndef _POINT_HPP_
#define _POINT_HPP_
#include <cstddef>

namespace gridworld_2D {
    struct point {
        int x_;
        int y_;

        int max_x_;
        int max_y_;

        bool operator==(const point& p) const {
            return p.x_ == x_ && p.y_ ==y_;
        }
    };
}

#endif