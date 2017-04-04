#ifndef ENSEMBLE_SCORE_H
#define ENSEMBLE_SCORE_H

#include "core/Score.hpp"

#include "detector/ensemble/CodeVector.hpp"

class EnsembleScore: public Score {
private:
public:
    CodeVector* binaryCodes;

    vector<double> scores;
    bool isMerged;

    EnsembleScore(int nrOfModels, int nrOfBaseClassifiers);
    EnsembleScore(vector<double> scores);

    void setBinaryCode(int index, int code);
    int getBinaryCode(int index);
    CodeVector* getCodeVector();
    double getProbability(int modelId);
    double getValue(int modelId);

    Score* clone();
    Score* sum(Score* other);
    Score* divide(int n);
};

#endif
