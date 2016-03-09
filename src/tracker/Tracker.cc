#include "tracker/Tracker.hpp"

Tracker::Tracker() {
    termCriteria = new TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03);
    winSize = new Size(4, 4);
}

vector<tld::Point*> Tracker::generatePoints(Box* box) {
    throw "NotImplemented!";
}

Flow* Tracker::computeFlow(Frame* src, Frame* target, vector<tld::Point*> srcPoints) {
    int pointCount = (int) srcPoints.size();

    vector<Point2f> fromPoints;
    vector<Point2f> toPoints;
    vector<uchar> status(pointCount);
    vector<float> error(pointCount);

    for (int i = 0; i < pointCount; i++) {
        tld::Point* point = srcPoints[i];
        fromPoints.push_back(point->underlying);
    }

    calcOpticalFlowPyrLK(src->flowPyramid,
                         target->flowPyramid,
                         fromPoints,
                         toPoints,
                         status,
                         error,
                         *winSize,
                         5,
                         *termCriteria,
                         CV_LKFLOW_INITIAL_GUESSES | CV_LKFLOW_PYR_A_READY | CV_LKFLOW_PYR_B_READY,
                         0.0001);

    vector<tld::Point*> targetPoints;
    for (int i = 0; i < pointCount; i++) {
        tld::Point* srcPoint = srcPoints[i];
        Point2f toPoint = toPoints[i];
        tld::Point* targetPoint = new tld::Point(srcPoint->id, toPoint);
        targetPoints.push_back(targetPoint);
    }

    Flow* flow = new Flow(src, target, srcPoints, targetPoints);
    return flow;
}

bool Tracker::isValid(Box* box, Frame* frame) {
    throw "NotImplemented!";
}

TrackResult* Tracker::track(Frame* prev, Frame* current, Box* box) {
    if (isValid(box, prev)) {
        vector<tld::Point*> points = generatePoints(box);

        Flow* forward = computeFlow(prev, current, points);
        Flow* backward = computeFlow(current, prev, forward->newPoints);

        FBFlow* fbFlow = new FBFlow(forward, backward);

        Option<ScoredBox>* maybeCurrentBox = fbFlow->estimate(box);
        TrackResult* trackResult = new TrackResult(maybeCurrentBox);
        return trackResult;
    } else {
        return TrackResult::empty;
    }
}
