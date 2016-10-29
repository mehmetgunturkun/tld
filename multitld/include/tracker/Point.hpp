#ifndef POINT_H
#define POINT_H

#include "cv.h"

using namespace std;

namespace tld {
    class Point {
    public:
        bool state;
        CvPoint2D32f* underlying;

        Point();
        Point(float x, float y);
        Point(CvPoint2D32f* underlying);

        string toString();

        static Point* failed;
    };
}
#endif
