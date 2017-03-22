#ifndef SCORED_BOX_H
#define SCORED_BOX_H

#include <string>
#include <unordered_map>

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
};
#endif
