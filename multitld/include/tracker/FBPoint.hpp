#ifndef FB_POINT_H
#define FB_POINT_H

#include "tracker/Point.hpp"

class FBPoint {
public:
    bool state;
    tld::Point* src;
    tld::Point* to;
    tld::Point* backwardPoint;

    float fbError;
    float ncc;
    float dx;
    float dy;

    FBPoint();
    FBPoint(tld::Point* src, tld::Point* to, tld::Point* bwPoint, float fbErr, float nccSim);
    string toString();
    const char* toCharArray();
    static FBPoint* failed;
};

#endif
