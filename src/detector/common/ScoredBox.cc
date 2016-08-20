#include "detector/common/ScoredBox.hpp"

ScoredBox::ScoredBox(Box* _box) {
    box = _box;
    isDetected = false;
}

void ScoredBox::withScore(string scorerKey, ClassificationDetails* detail) {
    detailMap[scorerKey] = detail;
}

void ScoredBox::merge(ScoredBox* other) {
    this->box = Box::merge(this->box, other->box);
    for ( auto it = this->detailMap.begin(); it != this->detailMap.end(); ++it ) {
        ClassificationDetails* thisDetail = it->second;
        ClassificationDetails* otherDetail = other->detailMap[it->first];
        ClassificationDetails* newScore = thisDetail->merge(otherDetail);
        this->detailMap[it->first] = newScore;
    }
}

ScoredBox* ScoredBox::clone() {
    Box* newBox = this->box->clone();
    ScoredBox* newScoredBox = new ScoredBox(newBox);
    newScoredBox->isDetected = this->isDetected;
    newScoredBox->detailMap = this->detailMap;

    return newScoredBox;
}
