#ifndef TLD_H
#define TLD_H

#include "tracker/Tracker.hpp"
#include "detector/Detector.hpp"

#include "tld/TrackerResult.hpp"
#include "tld/DetectorResult.hpp"

class TLDResultSet {
public:
    int modelId;
    Box* maybeTrackedBox;
    vector<ScoredBox*> scoredBoxList;

    TLDResultSet(Box* maybeTrackedBox) {
        this->maybeTrackedBox = maybeTrackedBox;
    }
    void add(ScoredBox* scoredBox) {
        this->scoredBoxList.push_back(scoredBox);
    }
};

class TLD {
public:
    Frame* firstFrame;
    vector<Box*> firstBoxList;

    Tracker* tracker;
    Detector* detector;
    int nrOfModels;

    TLD();
    TLD(Tracker* tracker, Detector* detector);

    vector<Box*> init(Frame* frame, vector<Box*> boxList);

    vector<Box*> track(Frame* prev, Frame* curr, vector<Box*> prevBoxList);
    vector<TLDResultSet*> groupResults(vector<Box*> boxList, vector<ScoredBox*> scoredBoxList);
    Option<Box>* integrate(Frame* frame, Box* oldBox, Box* maybeTrackedBox, vector<ScoredBox*> scoredBoxList, int modelId);

    TrackerResult* validate(Frame* current, Box* oldBox, Box* trackedBox, int modelId);
    Option<ScoredBox>* validate2(Frame* current, Box* oldBox, Box* trackedBox, int modelId);
    DetectorResult* partition(vector<ScoredBox*> scoredBoxList, int modelId);

    vector<ScoredBox*> getMoreConfidentBoxList(ScoredBox* trackScoredBox, vector<ScoredBox*> detectedBoxes, int modelId);
    Box* combineClosestBoxes(ScoredBox* trackScoredBox, vector<ScoredBox*> detectedBoxes);
};
#endif
