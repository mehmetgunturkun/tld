#ifndef ENSEMBLE_SCORE_H
#define ENSEMBLE_SCORE_H

#include "core/Score.hpp"

class EnsembleScore: public Score {
public:
    vector<float> scores;
    vector<int> binaryCodes;

    EnsembleScore() {
        scores.resize(13);
        binaryCodes.resize(13);
    }

    Score* merge(Score* other) {
        EnsembleScore* otherEnsembleScore = (EnsembleScore*) other;
        EnsembleScore* newEnsembleScore = new EnsembleScore();

        for (int i = 0; i < TLDConfig::nrOfModels; i++) {
            float thisScore = scores[i];
            float otherScore = otherEnsembleScore->scores[i];
            newEnsembleScore->scores[i] = (thisScore + otherScore) / 2;
        }

        return newEnsembleScore;
    }

    float getProbability(int modelId) {
        return scores[modelId];
    }
};

#endif
