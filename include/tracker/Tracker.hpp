#ifndef TRACKER_H
#define TRACKER_H

#include "opencv2/video/tracking.hpp"

#include "core/Frame.hpp"
#include "core/Box.hpp"
#include "core/Point.hpp"

#include "tracker/Flow.hpp"
#include "tracker/FBFlow.hpp"
#include "tracker/TrackResult.hpp"

using namespace std;
using namespace cv;

class Tracker {
private:
    TermCriteria* termCriteria;
    Size* winSize;
public:
    vector<tld::Point*> generatePoints(Box* box);
    Flow* computeFlow(Frame* src, Frame* target, vector<tld::Point*> points);
    bool isValid(Box* box, Frame* frame);
    TrackResult* track(Frame* prev, Frame* current, Box* box);
    Tracker();
};

#endif
