#ifndef ENSEMBLE_SCORE_H
#define ENSEMBLE_SCORE_H

#include "core/Score.hpp"

class EnsembleScore: public Score {
public:
    vector<float> scores;
    vector<int> binaryCodes;

    EnsembleScore() {
        scores.resize(TLDConfig::nrOfModels);
        binaryCodes.resize(13);
    }

    EnsembleScore(vector<float> scores) {
        this->scores = scores;
    }

    Score* merge(Score* other) {
        int n = (int) scores.size();
        vector<float> newScores(n);

        EnsembleScore* otherEnsembleScore = (EnsembleScore*) other;
        EnsembleScore* newEnsembleScore = new EnsembleScore(newScores);

        for (int i = 0; i < n; i++) {
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
