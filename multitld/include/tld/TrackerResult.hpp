#ifndef TRACKER_RESULT_H
#define TRACKER_RESULT_H

class TrackerResult {
public:
    bool isValid;
    ScoredBox* scoredBox;

    TrackerResult() {
        isValid = false;
        scoredBox = NULL;
    }

    TrackerResult(ScoredBox* scoredBox) {
        this->isValid = true;
        this->scoredBox = scoredBox;
    }
};
#endif
