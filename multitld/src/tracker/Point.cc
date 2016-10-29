#include "tracker/Point.hpp"

tld::Point::Point(CvPoint2D32f* underlying) {
    this->underlying = underlying;
    this->state = true;
}
tld::Point::Point() {
    this->state = false;
}

tld::Point::Point(float x, float y) {
    this->state = true;
    underlying = (CvPoint2D32f*) cvAlloc(sizeof(CvPoint2D32f));
    underlying->x = x;
    underlying->y = y;
}

string tld::Point::toString() {
    stringstream ss;
    ss  << "Point("
        << state << ", "
        << underlying->x << ", "
        << underlying->y << ")";
    return ss.str();
}

tld::Point* tld::Point::failed = new Point();
