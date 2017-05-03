#include "tld/TrackerResult.hpp"

TrackerResult::TrackerResult() {
    isValid = false;
    scoredBox = NULL;
}

TrackerResult::TrackerResult(ScoredBox* scoredBox) {
    this->isValid = true;
    this->scoredBox = scoredBox;
}
