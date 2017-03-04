#ifndef DETECTOR_H
#define DETECTOR_H

#include "core/Frame.hpp"
#include "core/Box.hpp"
#include "core/ScoredBox.hpp"

#include "core/BoxIterator.hpp"
#include "common/BoundedPriorityQueue.hpp"
#include "common/BoundedSortedVector.hpp"

#include "detector/variance/VarianceScore.hpp"
#include "detector/ensemble/EnsembleClassifier.hpp"
#include "detector/nn/NearestNeighborClassifier.hpp"

class Detector {
public:
    Frame* firstFrame;
    Box* firstBox;
    int nrOfModels;

    double minimumVariance;
    vector<double> varianceList;

    // VarianceClassifier* vClassifier;
    EnsembleClassifier* eClassifier;
    NearestNeighborClassifier* nnClassifier;

    int maxScaleLimit;
    int minimumPatchSize;

    double positiveBoxOverlapThreshold;
    double negativeBoxOverlapThreshold;
    double varianceThreshold;

    int nrOfPositiveBoxes4EnsembleAtInitialization;
    // int nrOfNegativeBoxes4EnsembleAtInitialization;
    int nrOfPositiveBoxes4NNAtInitialization;
    int nrOfNegativeBoxes4NNAtInitialization;

    Detector();

    void initVarianceThresholds(Frame* frame, vector<Box*> boxList);
    bool checkVariance(Frame* frame, ScoredBox* scoredBox);

    bool isPositive(Box* box);
    bool isNegative(Box* box, double varianceThresholdPerModel);

    vector<Box*> init(Frame* frame, vector<Box*> boxList);
    Box* init(Frame* frame, Box* box, int modelId);
    Box* getClosestBox(Frame* frame, Box* box);

    vector<ScoredBox*> detect(Frame* frame);
    void learn(Frame* current, Box* box, vector<ScoredBox*> grids, int modelId);
    ScoredBox* validate(Frame* frame, Box* box, int modelId);

    vector<ScoredBox*> score(Frame* frame, vector<Box*> boxList);
    void score(Frame* frame, ScoredBox* scoredBox);

    bool evaluate(Frame* frame, Box* box, double minVariance, int modelId);
    void dumpDetector();
};
#endif
