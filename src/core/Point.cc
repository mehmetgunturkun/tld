#include "core/Point.hpp"
tld::Point::Point(int _id, Point2f _underlying) {
    id = _id;
    underlying = _underlying;
}

tld::Point::Point(int id, float x, float y) {
    this->id = id;
    underlying = cv::Point2f(x, y);
}
