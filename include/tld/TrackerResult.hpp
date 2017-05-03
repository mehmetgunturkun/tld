#ifndef TRACKER_RESULT_H
#define TRACKER_RESULT_H

#include "core/ScoredBox.hpp"

class TrackerResult {
public:
    bool isValid;
    ScoredBox* scoredBox;

    TrackerResult();
    TrackerResult(ScoredBox* scoredBox);
};
#endif
