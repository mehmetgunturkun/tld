#ifndef TRACKER_H
#define TRACKER_H

#include "opencv2/video/tracking.hpp"

#include "core/Frame.hpp"
#include "core/Box.hpp"
#include "core/Point.hpp"

#include "tracker/Flow.hpp"
#include "tracker/FBFlow.hpp"

using namespace std;
using namespace cv;

class Tracker {
private:
    TermCriteria* termCriteria;
    Size* winSize;
    int margin;
public:
    vector<tld::Point*> generatePoints(Box* box);
    Flow* computeFlow(Frame* src, Frame* target, vector<tld::Point*> points);
    float computeStep(float start, float end, int pointCount);
    bool isValid(Box* box, Frame* frame);
    Option<Box>* track(Frame* prev, Frame* current, Box* box);
    Tracker();
};

#endif
