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


    double x1 = (double) src->underlying.x;
    double y1 = (double) src->underlying.y;

    double x2 = (double) to->underlying.x;
    double y2 = (double) to->underlying.y;

    double x3 = (double) backwardPoint->underlying.x;
    double y3 = (double) backwardPoint->underlying.y;

    this->dx = x2 - x1;
    this->dy = y2 - y1;

    //
    // double ddx = x3 - x1;
    // double ddy = y3 - y1;
    // this->fbError = sqrt(dx*dx + dy*dy);
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
