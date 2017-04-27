#ifndef LK_TRACKER_H
#define LK_TRACKER_H

#include <vector>

#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/video/tracking.hpp"

#include "common/Logging.hpp"

#include "core/Frame.hpp"
#include "core/Box.hpp"

#include "tracker/FBPoint.hpp"
#include "tracker/Tracker.hpp"

using namespace std;

class LKTracker: public Tracker {
private:
    int margin;
    TermCriteria* termCriteria;
    Size* winSize;

    float fbErrorThreshold;
public:
    LKTracker();
    ~LKTracker();
    vector<Box*> track(Frame* prev, Frame* curr, vector<Box*> boxList);

    vector<Box*> getStableBoxes(int nrOfBoxes, vector<Box*> boxList);
    vector<CvPoint2D32f*> decomposePoints(int nrOfBoxes, vector<Box*> boxList);
    float computeStep(float start, float end, int pointCount);
    vector<FBPoint*> track(Frame* prev, Frame* curr, int nrOfPoints, vector<CvPoint2D32f*> points);

    vector<Box*> fragmentAndEstimateBoxList(Frame* prev,
                                            Frame* curr,
                                            int nrOfStableBoxes,
                                            vector<Box*> stableBoxList,
                                            int nrOfBoxes,
                                            vector<Box*> boxList,
                                            vector<FBPoint*> trackedPoints);
    Option<Box*> estimate(Frame* prev, Frame* curr, Box* box, vector<FBPoint*> trackedPoints, int start, int end);

};

#endif
