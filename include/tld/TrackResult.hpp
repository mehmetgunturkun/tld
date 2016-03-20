#ifndef TRACK_RESULT_HPP
#define TRACK_RESULT_HPP

#include "common/Option.hpp"
#include "core/Box.hpp"
#include "detector/ScoredBox.hpp"

class TrackResult {
public:
    bool isFailed;
    ScoredBox* box;

    static TrackResult* empty;

    TrackResult();
    TrackResult(ScoredBox* b);
    TrackResult(Option<ScoredBox>* boxOpt);

    ScoredBox* getBox();
};
#endif
