#include "tracker/FBPoint.hpp"

FBPoint* FBPoint::failed = new FBPoint();

FBPoint::FBPoint() {
    this->state = false;
}

FBPoint::FBPoint(tld::Point* src, tld::Point* to, tld::Point* bwPoint) {
    this->state = true;
    this->src = src;
    this->to = to;
    this->backwardPoint = bwPoint;

    dx = to->underlying.x - src->underlying.x;
    dy = to->underlying.y - src->underlying.y;
}

string FBPoint::toString() {
    stringstream ss;
    if (this->state) {
        ss  << "FBPoint\n"
            << "\t" << this->src->toString() << "\n"
            << "\t" << this->to->toString() << "\n"
            << "\t" << this->backwardPoint->toString();
    } else {
        ss  << "FBPoint(failed)";
    }

    return ss.str();
}

const char* FBPoint::toCharArray() {
    return this->toString().c_str();
}
