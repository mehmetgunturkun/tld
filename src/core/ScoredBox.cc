#include "core/ScoredBox.hpp"

ScoredBox::ScoredBox(Box* _box) {
    box = _box;
}

void ScoredBox::withScore(string scorerKey, double score) {}
