#ifndef FB_POINT_H
#define FB_POINT_H

#include "core/Point.hpp"

using namespace tld;

class FBPoint {
    bool status;
    tld::Point* src;
    tld::Point* dest;
    tld::Point* fb;
};
#endif
