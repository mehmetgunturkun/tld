#ifndef DETECTOR_HPP
#define DETECTOR_HPP

#include "core/Frame.hpp"
#include "core/Box.hpp"
#include "detector/DetectResult.hpp"
#include "detector/training/TrainingSet.hpp"
#include "detector/common/BoxIterator.hpp"
#include "common/BoundedPriorityQueue.hpp"

#include "detector/ensemble/EnsembleClassifier.hpp"
#include "detector/nn/NearestNeighborClassifier.hpp"

class CascadedSingleDetector {
private:
    Frame* firstFrame;
    Box* firstBox;

    double positiveBoxOverlapThreshold;
    double negativeBoxOverlapThreshold;
    double varianceThreshold;

    bool isPositive(Box* box);
    bool isNegative(Box* box);

    EnsembleClassifier* eClassifier;
    NearestNeighborClassifier* nnClassifier;
public:
    CascadedSingleDetector();
    void init(Frame* frame, Box* box);
    DetectResult* detect(Frame* frame);
    ClassificationDetails* score(Frame* frame, Box* box);
    void learn(Frame* frame, Box* box, DetectResult* detectResult);
};
#endif
