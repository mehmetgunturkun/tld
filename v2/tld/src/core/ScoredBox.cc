#include "core/ScoredBox.hpp"

ScoredBox::ScoredBox(Box* box) {
    this->box = box;
    this->isDetected = false;
}

ScoredBox::~ScoredBox() {
    for ( auto it = scoreMap.begin(); it != scoreMap.end(); ++it ) {
        Score* score = it->second;
        delete score;
    }
}

ScoredBox* ScoredBox::withScore(string key, Score* score) {
    scoreMap[key] = score;
    return this;
}

Score* ScoredBox::getScore(string key) {
    return scoreMap[key];
}

double ScoredBox::getScoreValue(string key, int modelId) {
    Score* score = scoreMap[key];
    return score->getValue(modelId);
}

bool ScoredBox::isClassified(string classifierKey, int modelId) {
    Score* score = getScore(classifierKey);
    if (score == NULL) {
        return false;
    } else {
        vector<int> classifiedModelIds = score->classifiedModelIds;
        return std::find(classifiedModelIds.begin(), classifiedModelIds.end(), modelId) != classifiedModelIds.end();
    }
}

bool ScoredBox::isScored(string classifierKey) {
    Score* scoreItem = getScore(classifierKey);
    return scoreItem != NULL;
}
