#ifndef TLD_H
#define TLD_H

#include "tracker/Tracker.hpp"
#include "detector/Detector.hpp"

class TLD {
public:
    Tracker* tracker;
    Detector* detector;

    TLD(Tracker* tracker, Detector* detector);
    TLD(Frame* frame, vector<Box*> boxList);

    vector<Box*> track(Frame* prev, Frame* curr, vector<Box*> boxList);
};
#endif
