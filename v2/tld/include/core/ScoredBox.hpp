#ifndef SCORED_BOX_H
#define SCORED_BOX_H

#include <string>
#include <unordered_map>
#include <algorithm>

#include "core/Box.hpp"
#include "core/Score.hpp"

class ScoredBox {
public:
    Box* box;
    bool isDetected;
    unordered_map<string, Score*> scoreMap;

    ScoredBox(Box* box);
    ~ScoredBox();

    ScoredBox* withScore(string key, Score* score);
    Score* getScore(string key);
    double getScoreValue(string key, int modelId);

    bool isClassified(string classifierKey, int modelId);
    bool isScored(string classifierKey);
};

struct ScoredBoxOverlapOrdered {
    bool operator() (ScoredBox* box1, ScoredBox* box2) {
        return box1->box->overlap >= box2->box->overlap;
    }

    bool compare(ScoredBox* box1, ScoredBox* box2) {
        return box1->box->overlap > box2->box->overlap;
    }
};
#endif
