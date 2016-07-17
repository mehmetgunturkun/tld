#ifndef ENSEMBLE_CLASSIFIER_H
#define ENSEMBLE_CLASSIFIER_H

#include "core/Random.hpp"
#include "core/Frame.hpp"
#include "core/ScoredBox.hpp"

#include "detector/ensemble/BaseClassifier.hpp"
#include "detector/training/TrainingSet.hpp"

class EnsembleClassifier {
private:
    string classifierName;
    int nrOfBaseClassifiers;
    int nrOfPixelComparisons;
    int nrOfModels;
    vector<BaseClassifier*> baseClassifiers;

    vector<BaseClassifier*> generateBaseClassifier();
    vector<PixelComparison*> produceAllComparisons();
    vector<BaseClassifier*> shuffleComparisons(vector<PixelComparison*> allComparisons);

    float POSITIVE_SCORE_THRESHOLD;
public:
    EnsembleClassifier();
    bool classify(Frame* frame, ScoredBox* scoredBox);
    void train(TrainingSet<Box> ts, int modelId);
    void train(TrainingSet<ScoredBox> ts, int modelId);
};
#endif
