#ifndef FB_POINT_H
#define FB_POINT_H

#include <string>
#include <sstream>

#include "common/Logging.hpp"
#include "opencv2/core/types_c.h"

using namespace std;

class FBPoint {
public:
    bool state;
    CvPoint2D32f* src;
    CvPoint2D32f* to;
    CvPoint2D32f* backwardPoint;

    float fbError;
    float ncc;
    float dx;
    float dy;

    FBPoint();
    FBPoint(CvPoint2D32f* src, CvPoint2D32f* to, CvPoint2D32f* bwPoint, float fbErr, float nccSim);
    ~FBPoint();

    string toString();
    const char* toCharArray();

    static FBPoint* failed;
};

#endif
