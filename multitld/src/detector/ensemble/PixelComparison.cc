#include "detector/ensemble/PixelComparison.hpp"

PixelComparison::PixelComparison(Point2f* f, Point2f* t) {
    from = f;
    to = t;
}

Point2f* PixelComparison::normalize(Point2f* point, Box* box) {
    Point2f* normPoint = new Point2f(
        box->x1 + floor(point->x * (box->width - 1) + 0.5),
        box->y1 + floor(point->y * (box->height - 1) + 0.5)
    );
    return normPoint;
}

bool PixelComparison::compare(Frame* frame, Box* box) {
    Point2f* normFrom = normalize(from, box);
    Point2f* normTo = normalize(to, box);

    int fromPixel = frame->get(normFrom, CV_LOAD_IMAGE_GAUSSIAN);
    int toPixel = frame->get(normTo, CV_LOAD_IMAGE_GAUSSIAN);

    free(normFrom);
    free(normTo);

    return fromPixel > toPixel;
}
