#ifndef ENSEMBLE_SCORE_H
#define ENSEMBLE_SCORE_H

#include "core/Score.hpp"

class EnsembleScore: public Score {
public:
    bool isAnyClassified;
    vector<float> scores;
    vector<int> binaryCodes;

    EnsembleScore() {
        scores.resize(13);
        binaryCodes.resize(13);
    }
};

#endif
