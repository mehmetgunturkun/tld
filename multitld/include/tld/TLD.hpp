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
    ScoredBox* validate(Frame* current, Box* trackedBox, int modelId);
    Option<Box>* integrate(Frame* current, Box* trackedBox, vector<ScoredBox*> allBoxes, vector<ScoredBox*> detectedBoxes, int modelId);
    bool isThereMoreConfidentOneBox(ScoredBox* trackScoredBox, vector<ScoredBox*> detectedBoxes);
    Box* combineClosestBoxes(ScoredBox* trackScoredBox, vector<ScoredBox*> detectedBoxes);
};
#endif
