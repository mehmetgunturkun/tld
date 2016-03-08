#include "tracker/Tracker.hpp"

vector<tld::Point*> Tracker::generatePoints(Box* box) {
    throw "NotImplemented!";
}

Flow* Tracker::computeFlow(Frame* src, Frame* target, vector<tld::Point*> points) {
    throw "NotImplemented!";
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
