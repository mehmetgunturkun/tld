#ifndef ENSEMBLE_CLASSIFIER_H
#define ENSEMBLE_CLASSIFIER_H

#include <string>
#include <vector>

#include "common/Random.hpp"
#include "core/Frame.hpp"
#include "core/ScoredBox.hpp"

#include "detector/training/BoxHull.hpp"
#include "detector/training/TrainingSet.hpp"

#include "detector/ensemble/CodeVector.hpp"
#include "detector/ensemble/EnsembleScore.hpp"

#include "detector/ensemble/PixelComparison.hpp"
#include "detector/ensemble/BaseClassifier.hpp"

#define POS_X_DIR 1
#define NEG_X_DIR 2
#define POS_Y_DIR 3
#define NEG_Y_DIR 4

using namespace std;

class EnsembleClassifier {
public:
    string classifierName;
    int nrOfBaseClassifiers;
    int nrOfPixelComparisons;
    int nrOfModels;

    double positiveLabelThreshold;
    double positiveUpdateThreshold;
    double negativeUpdateThreshold;

    vector<BaseClassifier*> baseClassifiers;

    EnsembleClassifier();
    ~EnsembleClassifier();

    void init(Frame* firstFrame, vector<Box*> boxList);

    vector<BaseClassifier*> generateBaseClassifier();
    vector<PixelComparison*> produceAllComparisons();
    vector<BaseClassifier*> shuffleComparisons(vector<PixelComparison*> allComparisons);

    bool classify(Frame* frame, ScoredBox* scoredBox);
    CodeVector* generateBinaryCode(Frame* frame, Box* box);
    double getProbability(CodeVector* codeVector, int modelId);

    void train(TrainingSet<Box> ts, int modelId, double varianceThreshold);
    vector<Labelled<CodeVector>*> generateSamples(
        Frame* frame,
        vector<Box*> positiveBoxList,
        vector<Box*> negativeBoxList,
        double varianceThreshold);

    void train(TrainingSet<ScoredBox> ts, int modelId, double varianceThreshold);
    vector<Labelled<CodeVector>*> generateSamples(
        Frame* frame,
        vector<ScoredBox*> positiveBoxList,
        vector<ScoredBox*> negativeBoxList,
        double varianceThreshold);

    void updateBaseClassifiers(CodeVector* codeVector, int modelId, bool label);
    void doTrain(vector<Labelled<CodeVector>*> samples, int modelId);

};
#endif
