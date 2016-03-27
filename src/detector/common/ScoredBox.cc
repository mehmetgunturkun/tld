#include "detector/common/ScoredBox.hpp"

ScoredBox::ScoredBox(Box* _box) {
    box = _box;
    isDetected = false;
}

void ScoredBox::withScore(string scorerKey, ClassificationDetails* detail) {}

void merge(ScoredBox* other) {

}
