#ifndef ENSEMBLE_SCORE_H
#define ENSEMBLE_SCORE_H

#include "core/Score.hpp"

class EnsembleScore: public Score {
public:
    vector<float> scores;
    vector<int> binaryCodes;

    EnsembleScore() {}
};

#endif
