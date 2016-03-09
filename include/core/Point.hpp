#ifndef POINT_H
#define POINT_H

#include "opencv2/core/core.hpp"

namespace tld {
    class Point {
    public:
        int id;
        cv::Point2f underlying;

        Point(int id, Point2f underlying);
    };
}
#endif
