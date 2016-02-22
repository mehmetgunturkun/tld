#ifndef TRACKER_HPP
#define TRACKER_HPP

#include "core/Frame.hpp"
#include "core/Box.hpp"

#include "tracker/TrackResult.hpp"

class Tracker {
    TrackResult* track(Frame* prev, Frame* current, Box* boxOpt);
};

#endif
