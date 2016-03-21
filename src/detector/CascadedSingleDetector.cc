#include "detector/CascadedSingleDetector.hpp"

bool isPositive(Box* box) {
    return true;
}

bool isNegative(Box* box) {
    return true;
}

void CascadedSingleDetector::init(Frame* frame, Box* box) {
    BoundedPriorityQueue<Box*> positiveQueue = new BoundedPriorityQueue<Box*>(size = 10);
    BoundedPriorityQueue<Box*> negativeQueue = new BoundedPriorityQueue<Box*>();

    MeanVariance* initialMeanVariance = frame->integral->computeMeanVariance(
        (int) box->x1,
        (int) box->y1,
        (int) box->width,
        (int) box->height
    );

    box->mean = initialMeanVariance->mean;
    box->variance = initialMeanVariance->variance;

    BoxIterator* boxIterator = new BoxIterator(frame, box, 10, 24);
    while (boxIterator->hasNext()) {
        Box* sampleBox = boxIterator->next();

        //Compute Overlap
        double overlap = Box::computeOverlap(sampleBox, box);
        sampleBox->overlap = overlap;

        //Check if positive
        if (isPositive(box)) {
            positiveQueue += box;
            continue;
        }

        //Compute Variance
        MeanVariance* meanVariance = frame->integral->computeMeanVariance(
            (int) sampleBox->x1,
            (int) sampleBox->y1,
            (int) sampleBox->width,
            (int) sampleBox->height
        );

        sampleBox->mean = meanVariance->mean;
        sampleBox->variance = meanVariance->variance;

        //Check if negative
        if (isNegative(box)) {
            // Add to TrainingSetBuilder as positive
            negativeQueue += box;
            continue;
        }
    }

    vector<Box*> positiveBoxList4Ensemble = positiveQueue.toVector();
    vector<Box*> negativeBoxList4Ensemble = negativeQueue.toVector();
    TrainingSet<Box*> trainingSet4Ensemble = new TrainingSet<Box*>(
        positiveBoxList4Ensemble,
        negativeBoxList4Ensemble
    );

    Box* theClosestBox = positiveQueue.head();
    vector<Box*> positiveBoxList4NN = { theClosestBox };
    vector<Box*> negativeBoxList4NN = randomize(negativeBoxList, take = 10);
    TrainingSet<Box*> trainingSet4NN = new TrainingSet<Box*>(
        positiveBoxList4NN,
        negativeBoxList4NN
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
