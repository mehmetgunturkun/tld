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
    this->box = Box::merge(this->box, other->box);
    // for ( auto it = this->detailMap.begin(); it != this->detailMap.end(); ++it ) {
    //     Score* thisScore = it->second;
    //     Score* otherScore = other->detailMap[it->first];
    //     Score* newScore = thisScore->merge(otherScore);
    //     this->detailMap[it->first] = newScore;
    // }
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
