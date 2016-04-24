#ifndef TLD_HPP
#define TLD_HPP

#include "common/Option.hpp"

#include "core/Frame.hpp"
#include "core/Box.hpp"
#include "detector/common/ScoredBox.hpp"

#include "tld/TrackResult.hpp"
#include "tracker/Tracker.hpp"
#include "detector/nn/NNClassificationDetails.hpp"
#include "detector/CascadedSingleDetector.hpp"


using namespace std;

class TLD {
private:
    double minOverlap;
    double minValidationScore;

    TrackResult* validate(Frame* current, Option<Box>* trackResult);
    Option<Box>* integrate(Frame* current, TrackResult* trackResult, DetectResult* detectResult);
    Box* combineClosestBoxes(TrackResult* trackResult, DetectResult* detectResult);
    bool isThereMoreConfidentOneBox(TrackResult* trackResult, DetectResult* detectResult);
public:
    Tracker* tracker;
    CascadedSingleDetector* detector;

    TLD(Tracker* t, CascadedSingleDetector* d);
    TLD(Frame* f, Box* b);
    void init(Frame* firstFrame, Box* firstBox);
    TrackResult* track(Frame* prev, Frame* current, Option<Box>* maybePrevBox);
    void learn(Frame* current, Box* trackedBox, DetectResult* detectResult);
    DetectResult* detect(Frame* current);
    Option<Box>* process(Frame* prev, Frame* current, Option<Box>* maybePrevBox);
};
#endif
