#include "multitld/tracker/Tracker.hpp"

float Tracker::computeStep(float start, float end, int pointCount) {
    return ((end - MARGIN - 1) - (start + MARGIN)) / (pointCount - 1);
}

vector<tld::Point*> Tracker::generatePoints(vector<Box*> boxList, int nrOfBoxes) {
    vector<tld::Point*> points;
    int id = 0;
    for (int i = 0; i < nrOfBoxes; i++) {
        Box* box = boxList[i];
        int nrOfPoints = 0;
        float horizontalStep = computeStep(box->x1, box->x2, 10);
        float verticalStep = computeStep(box->y1, box->y2, 10);
        for (float j = MARGIN; j <= box->height - MARGIN; j = j + verticalStep) {
            for (float i = MARGIN; i <= box->width - MARGIN; i = i + horizontalStep) {
                Point2f point2f = Point2f(box->x1 + i, box->y1 + j);
                tld::Point* point = new tld::Point(id, point2f);

                nrOfPoints += 1;
                points.push_back(point);
                id++;
            }
        }
        box->nrOfPoints = nrOfPoints;
    }
    return points;
}

vector<FBPoint*> Tracker::lkTrack(vector<tld::Point*> points) {
    vector<FBPoint*> fbPoints;
    return fbPoints;
}

vector<Box*> Tracker::estimate(Frame* prev, Frame* curr, vector<Box*> boxList, vector<FBPoint*> trackedPoints) {
    vector<Box*> estimatedBoxList;
    return estimatedBoxList;
}

vector<Box*> Tracker::track(Frame* prev, Frame* curr, vector<Box*> boxList) {
    int nrOfBoxes = boxList.size();
    vector<tld::Point*> points = generatePoints(boxList, nrOfBoxes);
    vector<FBPoint*> trackedPoints = lkTrack(points);
    vector<Box*> estimatedBoxList = estimate(prev, curr, boxList, trackedPoints);
    return estimatedBoxList;
}
