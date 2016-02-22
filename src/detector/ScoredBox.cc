#include "detector/ScoredBox.hpp"

ScoredBox::ScoredBox(Box* _box) {
    box = _box;
}

void ScoredBox::withEnsembleScore(double eScore) {
    ensembleScore = eScore;
}

void ScoredBox::withNNScore(double nearestNeighborScore) {
    nnScore = nearestNeighborScore;
}
