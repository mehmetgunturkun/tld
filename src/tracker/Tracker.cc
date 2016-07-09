#include "tracker/Tracker.hpp"
#define MARGIN 5

Tracker::Tracker() {
    termCriteria = new TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03);
    winSize = new Size(4, 4);
    margin = 5;
}

Option<Box>* Tracker::track(Frame* prev, Frame* current, Box* box) {
    if (isValid(box, prev)) {
        vector<tld::Point*> points = generatePoints(box);

        printf("Number of points to track - %d - begin\n", points.size());
        for (int i = 0; i < points.size(); i++) {
            printf("%3d. %s\n", i, points[i]->toString().c_str());
        }
        printf("Number of points to track - %d - end\n", points.size());

        Flow* forward = computeFlow(prev, current, points);

        vector<Displacement*> displacementList = forward->displacementList;
        cout << "-------- FFF ----------\n";
        for (int i = 0; i < displacementList.size(); i++) {
            Displacement* d = displacementList[i];
            tld::Point* p1 = d->source;
            tld::Point* p2 = d->target;
            cout << p1->toString() << "\n";
            cout << p2->toString() << "\n";
            cout << "------------------\n";
        }

        Flow* backward = computeFlow(current, prev, forward->newPoints);

        FBFlow* fbFlow = new FBFlow(forward, backward);
        Option<Box>* maybeCurrentBox = fbFlow->estimate(box);
        return maybeCurrentBox;
    } else {
        Option<Box>* failed = new Option<Box>();
        return failed;
    }
}

bool isInside(Box* b, Frame* f) {
    return b->x1 < f->width && b->y1 < f->height && b->x2 > 0.0 && b->y2 > 0.0;
}

bool Tracker::isValid(Box* box, Frame* frame) {
    return isInside(box, frame);
}

vector<tld::Point*> Tracker::generatePoints(Box* box) {
    vector<tld::Point*> points;
    int id = 0;
    float horizontalStep = computeStep(box->x1, box->x2, 10);
    float verticalStep = computeStep(box->y1, box->y2, 10);
    for (float j = margin; j <= box->height - margin; j = j + verticalStep) {
        for (float i = margin; i <= box->width - margin; i = i + horizontalStep) {
            Point2f point2f = Point2f(box->x1 + i, box->y1 +j);
            tld::Point* point = new tld::Point(id, point2f);

            points.push_back(point);
            id++;
        }
    }
    return points;
}

float Tracker::computeStep(float start, float end, int pointCount) {
    return ((end - margin - 1) - (start + margin)) / (pointCount - 1);
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
