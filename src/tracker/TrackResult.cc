#include "tracker/Tracker.hpp"

TrackResult* TrackResult::empty = new TrackResult();

TrackResult::TrackResult() {
    isFailed = false;
}

TrackResult::TrackResult(ScoredBox* box) {
    isFailed = true;
    boxOpt = new Option<ScoredBox>(box);
}

ScoredBox* TrackResult::getBox() {
    return boxOpt->get();
}
