#include "tld/TLD.hpp"

TLD::TLD(Tracker* t, Detector* d) {
    tracker = t;
    detector = d;
}

void TLD::init(Frame* firstFrame, Box* firstBox) {

}

Option<Box>* TLD::process(Frame *prev, Frame *current, Option<Box> *prevBox) {
    TrackResult* trackResult = tracker->track(prev, current, prevBox);
    DetectResult* detectResult = detector->detect(current);
    Option<Box>* currentBox = integrate(current, trackResult, detectResult);
    return currentBox;
}

Option<Box>* TLD::integrate(Frame* current, TrackResult* trackResult, DetectResult* detectResult) {
    Option<Box>* none = new Option<Box>();
    return none;
}
