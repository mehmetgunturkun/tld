#ifndef DETECTOR_H
#define DETECTOR_H

#include "core/Frame.hpp"
#include "core/Box.hpp"
#include "core/ScoredBox.hpp"

#include "core/BoxIterator.hpp"
#include "common/BoundedPriorityQueue.hpp"
#include "detector/variance/VarianceClassifier.hpp"
#include "detector/ensemble/EnsembleClassifier.hpp"

class Detector {
public:
    Frame* firstFrame;
    Box* firstBox;

    VarianceClassifier* vClassifier;
    EnsembleClassifier* eClassifier;

    int maxScaleLimit;
    int minimumPatchSize;

    double positiveBoxOverlapThreshold;
    double negativeBoxOverlapThreshold;
    double varianceThreshold;

    int nrOfPositiveBoxes4EnsembleAtInitialization;
    int nrOfNegativeBoxes4EnsembleAtInitialization;

    Detector(Frame* frame, vector<Box*> boxList);

    bool isPositive(Box* box);
    bool isNegative(Box* box);
    void init(Frame* frame, Box* box);
    vector<ScoredBox*> detect(Frame* frame);
};
#endif
