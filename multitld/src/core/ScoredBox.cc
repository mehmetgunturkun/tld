#include "core/ScoredBox.hpp"

ScoredBox::ScoredBox() {
    this->isDetected = false;
}

ScoredBox::ScoredBox(Box* box) {
    this->box = box;
    this->isDetected = false;
}

ScoredBox* ScoredBox::clone() {
    //TODO NotImplemented
    ScoredBox* clone = new ScoredBox();
    clone->box = this->box->clone();
    clone->isDetected = this->isDetected;
    for ( auto it = this->detailMap.begin(); it != this->detailMap.end(); ++it ) {
        Score* score = it->second;
        clone->detailMap[it->first] = score->clone();
    }
    return clone;
}

ScoredBox* ScoredBox::sum(ScoredBox* other) {
    ScoredBox* mergedBox = new ScoredBox();
    mergedBox->box = this->box->sum(other->box);
    for ( auto it = this->detailMap.begin(); it != this->detailMap.end(); ++it ) {
        Score* thisScore = it->second;
        Score* otherScore = other->detailMap[it->first];

        Score* newScore = thisScore->sum(otherScore);
        mergedBox->detailMap[it->first] = newScore;
    }
    return mergedBox;
}

ScoredBox* ScoredBox::divide(int n) {
    this->box = this->box->divide(n);
    for ( auto it = this->detailMap.begin(); it != this->detailMap.end(); ++it ) {
        Score* thisScore = it->second;
        thisScore = thisScore->divide(n);
    }
    return this;
}

void ScoredBox::withScore(string classifierKey, Score* score) {
    detailMap[classifierKey] = score;
}

Score* ScoredBox::getScore(string classifierKey) {
    Score* score = detailMap[classifierKey];
    return score;
}

double ScoredBox::getScoreValue(string classifierKey, int modelId) {
    Score* score = getScore(classifierKey);
    return score->getValue(modelId);
}

vector<int> ScoredBox::getCandidateModels(string classifierKey) {
    Score* score = getScore(classifierKey);
    return score->classifiedModelIds;
}

vector<int> ScoredBox::getCandidateModels() {
    Score* score = getScore("nn");
    return score->classifiedModelIds;
}

bool ScoredBox::isClassified(string classifierKey, int modelId) {
    Score* score = getScore(classifierKey);
    if (score == NULL) {
        printf("There is no score for %s\n", classifierKey.c_str());
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
