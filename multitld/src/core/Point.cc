#include "core/Point.hpp"

tld::Point::Point(cv::Point2f underlying) {
    this->underlying = underlying;
    this->state = true;
}
tld::Point::Point() {
    this->state = false;
}

tld::Point::Point(float x, float y) {
    this->state = true;
    underlying = cv::Point2f(x, y);
}

string tld::Point::toString() {
    stringstream ss;
    ss  << "Point("
        << state << ", "
        << underlying.x << ", "
        << underlying.y << ")";
    return ss.str();
}

tld::Point* tld::Point::failed = new Point();
