#include "detector/ensemble/PixelComparison.hpp"

PixelComparison::PixelComparison(Point2f* f, Point2f* t) {
    from = f;
    to = t;
}

PixelComparison::~PixelComparison() {
    delete from;
    delete to;
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
    int fromPixel = frame->get(normFrom);

    Point2f* normTo = normalize(to, box);
    int toPixel = frame->get(normTo);

    delete normFrom;
    delete normTo;

    return fromPixel > toPixel;
}

string PixelComparison::toString() {
    stringstream ss;
    ss  << "PC("
        << "P1("
        << from->x << ", "
        << from->y << ")"
        << " -> "
        << "P2("
        << to->x << ", "
        << to->y << ")";
    return ss.str();
}
