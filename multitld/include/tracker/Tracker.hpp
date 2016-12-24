#ifndef TRACKER_H
#define TRACKER_H

#include "opencv2/video/tracking.hpp"
#include "common/Option.hpp"

#include "core/Frame.hpp"
#include "core/Box.hpp"

#include "tracker/Point.hpp"
#include "tracker/FBPoint.hpp"

using namespace std;
using namespace tld;

class Tracker {
private:
    int MARGIN;
    TermCriteria* TERM_CRITERIA = new TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03);
    Size* WIN_SIZE = new Size(4, 4);
    float fbErrorThreshold;

    float computeStep(float start, float end, int pointCount);
public:
    Tracker();
    vector<tld::Point*> decomposePoints(vector<Box*> boxList, int nrOfBoxes);
    vector<FBPoint*> track(Frame* prev, Frame* curr, int nrOfPoints, vector<tld::Point*> points);
    vector<tld::Point*> lkTrack(Frame* prev, Frame* curr, vector<tld::Point*> points);
    vector<Box*> fragmentAndEstimateBoxList(Frame* prev,
                                            Frame* curr,
                                            int nrOfStableBoxes,
                                            vector<Box*> stableBoxList,
                                            int nrOfBoxes,
                                            vector<Box*> boxList,
                                            vector<FBPoint*> trackedPoints);
    vector<Box*> track(Frame* prev, Frame* curr, std::vector<Box*> boxList);
    vector<Box*> trackSilently(Frame* prev, Frame* curr, vector<Box*> boxList);
    Option<Box>* estimate(Frame* prev, Frame* curr, Box* box, vector<FBPoint*> trackedPoints, int start, int end);
};
#endif
