#ifndef TRACKER_RESULT_H
#define TRACKER_RESULT_H

class TrackerResult {
public:
    bool isValid;
    ScoredBox* scoredBox;
    TrackerResult(ScoredBox* scoredBox) {
        this->scoredBox = scoredBox;
        this->isValid = scoredBox->isDetected;
    }
};
#endif
