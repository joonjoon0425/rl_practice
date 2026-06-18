#ifndef _POINT_HPP_
#define _POINT_HPP_

namespace gridworld_2D {
    struct point {
        int x;
        int y;

        bool operator==(const point& p) const {
            return p.x == x && p.y ==y;
        }
    };
}

#endif