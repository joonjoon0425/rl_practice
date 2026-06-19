#ifndef _POINT_HPP_
#define _POINT_HPP_
#include <cstddef>

namespace gridworld_2D {
    struct point {
        int x_;
        int y_;

        int max_x_;
        int max_y_;

        point(int x, int y, int max_x, int max_y)
        : x_(x),
          y_(y),
          max_x_(max_x),
          max_y_(max_y)
        {
            
        }

        bool operator==(const point& p) const {
            return p.x_ == x_ && p.y_ ==y_;
        }
    };
}

#endif