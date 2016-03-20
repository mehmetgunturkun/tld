#include "tracker/Tracker.hpp"
#define MARGIN 5

Tracker::Tracker() {
    termCriteria = new TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03);
    winSize = new Size(4, 4);
}

Option<Box>* Tracker::track(Frame* prev, Frame* current, Box* box) {
    if (isValid(box, prev)) {
        vector<tld::Point*> points = generatePoints(box);

        Flow* forward = computeFlow(prev, current, points);
        Flow* backward = computeFlow(current, prev, forward->newPoints);

        FBFlow* fbFlow = new FBFlow(forward, backward);
        Option<Box>* maybeCurrentBox = fbFlow->estimate(box);
        return maybeCurrentBox;
    } else {
        Option<Box>* failed = new Option<Box>();
        return failed;
    }
}

float computeStep(float start, float end, int pointCount) {
    return ((end - MARGIN -1) - (start + MARGIN)) / (pointCount - 1);
}

vector<tld::Point*> Tracker::generatePoints(Box* box) {
    vector<tld::Point*> points;
    int id = 0;
    float horizontalStep = computeStep(box->x1, box->x2, 10);
    float verticalStep = computeStep(box->y1, box->y2, 10);
    for (float j = MARGIN; j <= box->height - MARGIN; j = j + verticalStep) {
        for (float i = MARGIN; i <= box->width - MARGIN; i = i + horizontalStep) {
            Point2f point2f = Point2f(box->x1 + i, box->y1 +j);
            tld::Point* point = new tld::Point(id, point2f);

            points.push_back(point);
            id++;
        }
    }
    return points;
}

Flow* Tracker::computeFlow(Frame* src, Frame* target, vector<tld::Point*> srcPoints) {
    int pointCount = (int) srcPoints.size();

    vector<Point2f> fromPoints;
    vector<Point2f> toPoints;
    vector<uchar> status(pointCount);
    vector<float> errors(pointCount);

    for (int i = 0; i < pointCount; i++) {
        tld::Point* point = srcPoints[i];
        fromPoints.push_back(point->underlying);
        toPoints.push_back(point->underlying);
    }

    calcOpticalFlowPyrLK(src->flowPyramid,
                         target->flowPyramid,
                         fromPoints,
                         toPoints,
                         status,
                         errors,
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

    Flow* flow = new Flow(src, target, srcPoints, targetPoints, status, errors);
    return flow;
}

bool Tracker::isValid(Box* box, Frame* frame) {
    return true;
}
