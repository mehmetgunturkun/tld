#include "core/ScoredBox.hpp"

ScoredBox::ScoredBox(Box* box) {

}

ScoredBox* ScoredBox::clone() {
    //TODO NotImplemented
    return this;
}

void ScoredBox::merge(ScoredBox* other) {

}

void ScoredBox::withScore(string classifierKey, Score* score) {

}

Score* ScoredBox::getScore(string classifierKey) {
    Score* score = detailMap[classifierKey];
    return score;
}

double ScoredBox::getScoreValue(string classifierKey) {
    Score* score = getScore(classifierKey);
    return score->value;
}
