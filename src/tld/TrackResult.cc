#include "tld/TrackResult.hpp"

TrackResult* TrackResult::empty = new TrackResult();

TrackResult::TrackResult() {
    isFailed = false;
}

TrackResult::TrackResult(ScoredBox* b) {
    isFailed = false;
    box = b;
}

TrackResult::TrackResult(Option<ScoredBox>* boxOpt) {
    if (boxOpt->isDefined()) {
        isFailed = false;
        box = boxOpt->get();
    } else {
        isFailed = true;
    }
}

ScoredBox* TrackResult::getBox() {
    return box;
}
