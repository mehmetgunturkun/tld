#ifndef ENSEMBLE_SCORE_H
#define ENSEMBLE_SCORE_H

#include "core/Score.hpp"

class EnsembleScore: public Score {
public:
    vector<float> scores;
    vector<int> binaryCodes;
    bool isMerged;

    EnsembleScore(int nrOfModels, int nrOfBaseClassifiers) {
        isMerged = false;
        scores.resize(nrOfModels);
        binaryCodes.resize(nrOfBaseClassifiers);
    }

    EnsembleScore(vector<float> scores) {
        isMerged = true;
        this->scores = scores;
    }

    Score* clone() {
        EnsembleScore* clone = new EnsembleScore(this->scores);
        clone->binaryCodes = this->binaryCodes;
        return clone;
    }

    Score* sum(Score* other) {
        EnsembleScore* otherEnsembleScore = (EnsembleScore*) other;
        EnsembleScore* newEnsembleScore = (EnsembleScore*) this->clone();

        int n = (int) scores.size();
        for (int i = 0; i < n; i++) {
            float thisScore = scores[i];
            float otherScore = otherEnsembleScore->scores[i];
            newEnsembleScore->scores[i] = thisScore + otherScore;
        }

        return newEnsembleScore;
    }

    Score* divide(int n) {
        int m = (int) scores.size();
        for (int i = 0; i < m; i++) {
            scores[i] = scores[i] / n;
        }
        return this;
    }

    float getProbability(int modelId) {
        return scores[modelId];
    }

    float getValue(int modelId) {
        return scores[modelId];
    }
};

#endif
