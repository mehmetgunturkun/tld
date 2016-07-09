#ifndef POINT_H
#define POINT_H

#include "opencv2/core/core.hpp"

using namespace std;
using namespace cv;

namespace multitld {
    class Point {
    public:
        bool state;
        cv::Point2f underlying;

        Point(Point2f underlying);
        Point();

        static multitld::Point* failed() {
            return new multitld::Point();
        }

        string toString() {
            stringstream ss;
            ss  << "Point("
                << underlying.x << ", "
                << underlying.y << ")";
            return ss.str();
        }
    };
}
#endif
