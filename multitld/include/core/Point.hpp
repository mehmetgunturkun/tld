#ifndef POINT_H
#define POINT_H

#include "opencv2/core/core.hpp"

using namespace std;

namespace tld {
    class Point {
    public:
        bool state;
        cv::Point2f underlying;

        Point();
        Point(float x, float y);
        Point(cv::Point2f underlying);

        string toString();

        static Point* failed;
    };
}
#endif
