#ifndef ENSEMBLE_SCORE_H
#define ENSEMBLE_SCORE_H

#include "core/Score.hpp"

#include "detector/ensemble/CodeVector.hpp"

class EnsembleScore: public Score {
private:
    CodeVector* binaryCodes;
public:
    vector<double> scores;
    bool isMerged;

    EnsembleScore(int nrOfModels, int nrOfBaseClassifiers) {
        isMerged = false;
        scores.resize(nrOfModels);
        binaryCodes = new CodeVector(nrOfBaseClassifiers);
    }

    EnsembleScore(vector<double> scores) {
        isMerged = true;
        this->scores = scores;
    }

    void setBinaryCode(int index, int code) {
        binaryCodes->set(index, code);
    }

    int getBinaryCode(int index) {
        return binaryCodes->get(index);
    }

    CodeVector* getCodeVector() {
        return binaryCodes;
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

    double getProbability(int modelId) {
        return scores[modelId];
    }

    double getValue(int modelId) {
        return scores[modelId];
    }
};

#endif
