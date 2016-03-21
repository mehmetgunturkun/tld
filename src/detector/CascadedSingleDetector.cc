#include "detector/CascadedSingleDetector.hpp"

bool CascadedSingleDetector::isPositive(Box* box) {
    return (box->overlap > positiveBoxOverlapThreshold);
}

bool CascadedSingleDetector::isNegative(Box* box) {
    return (box->overlap < negativeBoxOverlapThreshold) && (box->variance > varianceThreshold);
}

void CascadedSingleDetector::init(Frame* frame, Box* box) {
    firstFrame = frame;
    firstBox = box;

    positiveBoxOverlapThreshold = 0.6;
    negativeBoxOverlapThreshold = 0.2;

    MeanVariance* initialMeanVariance = firstFrame->integral->computeMeanVariance(
        (int) firstBox->x1,
        (int) firstBox->y1,
        (int) firstBox->width,
        (int) firstBox->height
    );
    varianceThreshold = initialMeanVariance->variance * 0.5;


    BoundedPriorityQueue<Box, OverlapOrdered> positiveQueue = BoundedPriorityQueue<Box, OverlapOrdered>(10);
    BoundedPriorityQueue<Box, OverlapOrdered> negativeQueue = BoundedPriorityQueue<Box, OverlapOrdered>(INT_MAX);

    BoxIterator* boxIterator = new BoxIterator(firstFrame, firstBox, 10, 24);
    while (boxIterator->hasNext()) {
        Box* sampleBox = boxIterator->next();

        //Compute Overlap
        double overlap = Box::computeOverlap(sampleBox, firstBox);
        sampleBox->overlap = overlap;

        //Check if positive
        if (isPositive(sampleBox)) {
            positiveQueue += sampleBox;
            continue;
        }

        //Compute Variance
        MeanVariance* meanVariance = firstFrame->integral->computeMeanVariance(
            (int) sampleBox->x1,
            (int) sampleBox->y1,
            (int) sampleBox->width,
            (int) sampleBox->height
        );

        sampleBox->mean = meanVariance->mean;
        sampleBox->variance = meanVariance->variance;

        //Check if negative
        if (isNegative(sampleBox)) {
            // Add to TrainingSetBuilder as positive
            negativeQueue += sampleBox;
            continue;
        }
    }

    vector<Box*> positiveBoxList4Ensemble = positiveQueue.toVector();
    vector<Box*> negativeBoxList4Ensemble = negativeQueue.toVector();
    TrainingSet<Box> trainingSet4Ensemble = TrainingSet<Box>(
        positiveBoxList4Ensemble,
        negativeBoxList4Ensemble
    );
}

DetectResult* CascadedSingleDetector::detect(Frame* frame) {
    DetectResult* detectResult = new DetectResult();
    return detectResult;
}

double CascadedSingleDetector::score(Frame* frame, Box* box) {
    return 0.0;
}

void CascadedSingleDetector::learn(Frame* frame, Box* box, DetectResult* detectResult) {
    throw "NotImplemented!";
}
