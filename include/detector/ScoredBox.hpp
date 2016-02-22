#ifndef SCORED_BOX_HPP
#define SCORED_BOX_HPP

#include "core/Box.hpp"

class ScoredBox {
    Box* box;
    double ensembleScore;
    double nnScore;

    ScoredBox(Box* box);

    void withEnsembleScore(double eScore);
    void withNNScore(double nearestNeighborScore);

};
#endif
