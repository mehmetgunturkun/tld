#include "core/ScoredBox.hpp"

ScoredBox::ScoredBox(Box* box) {
    this->box = box;
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
