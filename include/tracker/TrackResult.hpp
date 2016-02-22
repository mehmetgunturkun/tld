#ifndef TRACK_RESULT_HPP
#define TRACK_RESULT_HPP

#include "common/Option.hpp"
#include "core/Box.hpp"

class TrackResult {
    bool isFailed;
    Option<Box>* boxOpt;

    TrackResult();
    TrackResult(Box* box);
};
#endif
