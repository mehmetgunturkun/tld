#include "tracker/FBPoint.hpp"

FBPoint* FBPoint::failed = new FBPoint();

FBPoint::FBPoint() {
    this->state = false;
}

FBPoint::FBPoint(CvPoint2D32f* src, CvPoint2D32f* to, CvPoint2D32f* bwPoint, float fbErr, float nccSim) {
    this->state = true;
    this->src = src;
    this->to = to;
    this->backwardPoint = bwPoint;

    dx = to->x - src->x;
    dy = to->y - src->y;

    fbError = fbErr;
    ncc = nccSim;
}

FBPoint::~FBPoint() {
    println("mc1000");
    cvFree(&src);
    println("mc1100");
    cvFree(&to);
    println("mc1200");
    cvFree(&backwardPoint);
    println("mc1300");
}

string FBPoint::toString() {
    stringstream ss;
    // if (this->state) {
    //     ss  << "FBPoint\n"
    //         << "\t" << this->src->toString() << "\n"
    //         << "\t" << this->to->toString() << "\n"
    //         << "\t" << this->backwardPoint->toString();
    // } else {
    //     ss  << "FBPoint(failed)";
    // }

    return ss.str();
}

const char* FBPoint::toCharArray() {
    return this->toString().c_str();
}
