#ifndef SCORED_BOX_H
#define SCORED_BOX_H

#include <string>
#include <unordered_map>
#include <queue>

#include "common/BoundedPriorityQueue.hpp"
#include "core/Box.hpp"
#include "core/Score.hpp"

class ScoredBox {
public:
    Box* box;
    bool isDetected;
    unordered_map<string, Score*> detailMap;

    ScoredBox(Box* box);

    ScoredBox* clone();
    void merge(ScoredBox* other);
    void withScore(string classifierKey, Score* score);
    Score* getScore(string classifierKey);
    double getScoreValue(string classifierKey);
};
#endif
