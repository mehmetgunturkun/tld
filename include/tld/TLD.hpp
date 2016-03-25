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
    Option<Box>* integrate(Frame* current, TrackResult* trackResult, DetectResult* detectResult);
    Box* combineClosestBoxes(TrackResult* trackResult, DetectResult* detectResult);
    bool isThereMoreConfidentOneBox(TrackResult* trackResult, DetectResult* detectResult);
    TrackResult* validate(Frame* current, Option<Box>* trackResult);

    double MIN_OVERLAP;
    double MIN_VALIDATION_SCORE;

public:
    Tracker* tracker;
    CascadedSingleDetector* detector;

    TLD(Tracker* t, CascadedSingleDetector* d);

    void init(Frame* firstFrame, Box* firstBox);
    TrackResult* track(Frame* prev, Frame* current, Option<Box>* maybePrevBox);
    void learn(Frame* current, Box* trackedBox, DetectResult* detectResult);
    DetectResult* detect(Frame* current);
    Option<Box>* process(Frame* prev, Frame* current, Option<Box>* maybePrevBox);

};
#endif
