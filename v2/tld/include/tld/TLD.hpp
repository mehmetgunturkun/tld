#ifndef TLD_H
#define TLD_H
#include "core/Box.hpp"
#include "core/ScoredBox.hpp"

#include "tracker/Tracker.hpp"
#include "detector/Detector.hpp"

#include "tld/TrackerResult.hpp"
#include "tld/DetectorResult.hpp"

class TLDResultSet {
public:
    int modelId;
    Box* maybeTrackedBox;
    vector<ScoredBox*> scoredBoxList;

    TLDResultSet(Box* maybeTrackedBox);

    void add(ScoredBox* scoredBox);
};

class TLD {
public:
    int nrOfModels;

    Tracker* tracker;
    Detector* detector;

    TLD();
    TLD(Tracker* tracker, Detector* detector);

    vector<Box*> init(Frame* frame, vector<Box*> boxList);

    vector<Box*> track(Frame* prev, Frame* current, vector<Box*> prevBoxList);
    vector<TLDResultSet*> groupResults(vector<Box*> boxList, vector<ScoredBox*> scoredBoxList);
    Option<Box*> integrate(Frame* frame, Box* oldBox, Box* maybeTrackedBox, vector<ScoredBox*> scoredBoxList, int modelId);

    Option<ScoredBox*> validate(Frame* current, Box* oldBox, Box* trackedBox, int modelId);
    DetectorResult* partition(vector<ScoredBox*> scoredBoxList, int modelId);

    vector<ScoredBox*> getMoreConfidentBoxList(ScoredBox* trackScoredBox, vector<ScoredBox*> detectedBoxes, int modelId);
    Box* combineClosestBoxes(ScoredBox* trackScoredBox, vector<ScoredBox*> detectedBoxes);};

#endif
