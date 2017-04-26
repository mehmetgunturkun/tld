#ifndef SCORED_BOX_H
#define SCORED_BOX_H

#include <string>
#include <unordered_map>
#include <algorithm>

#include "common/clustering/Cluster.hpp"
#include "common/collection/BoundedPriorityQueue.hpp"

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
    Score* remove(string key);
    double getScoreValue(string key, int modelId);

    bool isClassified(string classifierKey, int modelId);
    bool isScored(string classifierKey);

    ScoredBox* clone();
    ScoredBox* sum(ScoredBox* other);
    ScoredBox* divide(int n);

    static vector<ScoredBox*> cluster(vector<ScoredBox*> boxList, int nrOfBoxes);
    static ScoredBox* merge(vector<ScoredBox*> scoredBoxList, int nrOfBoxes);
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
