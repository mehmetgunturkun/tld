#ifndef DETECTOR_H
#define DETECTOR_H

#include "common/Random.hpp"
#include "common/collection/BoundedSortedVector.hpp"

#include "core/Frame.hpp"
#include "core/Box.hpp"
#include "core/ScoredBox.hpp"

#include "detector/variance/VarianceScore.hpp"
#include "detector/core/BoxIterator.hpp"
#include "detector/ensemble/EnsembleClassifier.hpp"


class Detector {
public:
    int nrOfModels;
    Box* firstBox;

    double minimumVariance;
    vector<double> varianceList;

    int maxScaleLimit;
    int minimumPatchSize;

    double positiveBoxOverlapThreshold;
    double negativeBoxOverlapThreshold;

    int nrOfPositiveBoxes4EnsembleAtInitialization;
    // int nrOfNegativeBoxes4EnsembleAtInitialization;

    int nrOfPositiveBoxes4NNAtInitialization;
    int nrOfNegativeBoxes4NNAtInitialization;

    EnsembleClassifier* eClassifier;

    Detector();

    void initVarianceThresholds(Frame* frame, vector<Box*> boxList);
    bool checkVariance(Frame* frame, ScoredBox* scoredBox);

    bool isPositive(Box* box);
    bool isNegative(Box* box, double varianceThresholdPerModel);

    vector<Box*> init(Frame* frame, vector<Box*> boxList);
    Box* init(Frame* frame, Box* box, int modelId);
    Box* getClosestBox(Frame* frame, Box* box);

    vector<ScoredBox*> detect(Frame* frame);

    ScoredBox* validate(Frame* frame, Box* box, int modelId);
    bool evaluate(Frame* frame, Box* box, double minVariance, int modelId);

    vector<ScoredBox*> score(Frame* frame, vector<Box*> boxList);
    void score(Frame* frame, ScoredBox* scoredBox);

    void learn(Frame* current, Box* box, vector<ScoredBox*> grids, int modelId);
};
#endif
