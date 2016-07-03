#ifndef TRACKER_H
#define TRACKER_H

#include <vector>

#include "common/Option.hpp"

#include "core/Point.hpp"

#include "multitld/core/Frame.hpp"
#include "multitld/core/Box.hpp"
#include "multitld/core/FBPoint.hpp"

using namespace std;
using namespace tld;

class Tracker {
private:
    int MARGIN;
public:
    float computeStep(float start, float end, int pointCount);
    vector<tld::Point*> generatePoints(vector<Box*> boxList, int nrOfBoxes);
    vector<FBPoint*> lkTrack(vector<tld::Point*> points);
    vector<Box*> estimate(Frame* prev, Frame* curr, vector<Box*> boxList, vector<FBPoint*> trackedPoints);
    std::vector<Box*> track(Frame* prev, Frame* curr, std::vector<Box*> boxList);
};
#endif
