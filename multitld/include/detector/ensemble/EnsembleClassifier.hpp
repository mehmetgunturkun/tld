#ifndef ENSEMBLE_CLASSIFIER_H
#define ENSEMBLE_CLASSIFIER_H

#include "core/Random.hpp"
#include "core/Frame.hpp"
#include "core/ScoredBox.hpp"

#include "detector/ensemble/BaseClassifier.hpp"
#include "detector/training/TrainingSet.hpp"

class EnsembleClassifier {
public:
    string classifierName;
    int nrOfBaseClassifiers;
    int nrOfPixelComparisons;
    int nrOfModels;

    float positiveUpdateThreshold;
    float negativeUpdateThreshold;

    vector<BaseClassifier*> baseClassifiers;

    vector<BaseClassifier*> generateBaseClassifier();
    vector<PixelComparison*> produceAllComparisons();
    vector<BaseClassifier*> shuffleComparisons(vector<PixelComparison*> allComparisons);

    float POSITIVE_SCORE_THRESHOLD;

    EnsembleClassifier();
    void init(Frame* firstFrame, vector<Box*> boxList);
    bool classify(Frame* frame, ScoredBox* scoredBox);
    float getProbability(ScoredBox* scoredBox, int modelId);
    void score(Frame* frame, ScoredBox* scoredBox);
    void train(TrainingSet<Box> ts, int modelId);
    void train(TrainingSet<ScoredBox> ts, int modelId);
    void updateBaseClassifiers(Frame* frame, ScoredBox* scoredBox, int modelId, bool label);
    void dumpEnsembleClassifier();
};
#endif
