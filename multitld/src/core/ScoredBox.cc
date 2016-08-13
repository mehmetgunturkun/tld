#include "core/ScoredBox.hpp"

ScoredBox::ScoredBox(Box* box) {
    this->box = box;
    this->isDetected = false;
}

ScoredBox* ScoredBox::clone() {
    //TODO NotImplemented
    return this;
}

void ScoredBox::merge(ScoredBox* other) {
    //TODO NotImplemented
}

void ScoredBox::withScore(string classifierKey, Score* score) {
    detailMap[classifierKey] = score;
}

Score* ScoredBox::getScore(string classifierKey) {
    Score* score = detailMap[classifierKey];
    return score;
}

double ScoredBox::getScoreValue(string classifierKey) {
    Score* score = getScore(classifierKey);
    return score->value;
}

vector<int> ScoredBox::getCandidateModels() {
    Score* score = getScore("nn");
    return score->classifiedModelIds;
}
