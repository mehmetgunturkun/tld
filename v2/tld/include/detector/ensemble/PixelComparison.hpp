#ifndef PIXEL_COMPARISON_H
#define PIXEL_COMPARISON_H

#include "core/Frame.hpp"
#include "core/Box.hpp"

class PixelComparison {
    Point2f* from;
    Point2f* to;
public:
    PixelComparison(Point2f* fromPoint, Point2f* toPoint);
    Point2f* normalize(Point2f* point, Box* box);
    bool compare(Frame* frame, Box* box);
    string toString();
};
#endif
