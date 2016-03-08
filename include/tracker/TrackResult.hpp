#ifndef TRACK_RESULT_HPP
#define TRACK_RESULT_HPP

#include "common/Option.hpp"
#include "core/Box.hpp"
#include "core/ScoredBox.hpp"

class TrackResult {
public:
    bool isFailed;
    Option<ScoredBox>* boxOpt;

    static TrackResult* empty;

    TrackResult();
    TrackResult(ScoredBox* box);
    TrackResult(Option<ScoredBox>* box);
    ScoredBox* getBox();
};
#endif
