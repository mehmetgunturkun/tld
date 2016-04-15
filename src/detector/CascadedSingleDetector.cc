#include "detector/CascadedSingleDetector.hpp"

CascadedSingleDetector::CascadedSingleDetector() {
    eClassifier = new EnsembleClassifier();
    nnClassifier = new NearestNeighborClassifier();

    positiveBoxOverlapThreshold = 0.6;
    negativeBoxOverlapThreshold = 0.2;

    nrOfNegativeBoxes4EnsembleAtInitialization = 100;
    nrOfPositiveBoxes4EnsembleAtInitialization = 10;

    nrOfNegativeBoxes4NNAtInitialization = 10;
    nrOfPositiveBoxes4NNAtInitialization = 1;
}

CascadedSingleDetector::CascadedSingleDetector(EnsembleClassifier* ec, NearestNeighborClassifier* nnc) {
    eClassifier = ec;
    nnClassifier = nnc;

    positiveBoxOverlapThreshold = 0.6;
    negativeBoxOverlapThreshold = 0.2;

    nrOfNegativeBoxes4EnsembleAtInitialization = 100;
    nrOfPositiveBoxes4EnsembleAtInitialization = 10;

    nrOfNegativeBoxes4NNAtInitialization = 10;
    nrOfPositiveBoxes4NNAtInitialization = 1;
}

bool CascadedSingleDetector::isPositive(Box* box) {
    return (box->overlap > positiveBoxOverlapThreshold);
}

bool CascadedSingleDetector::isNegative(Box* box) {
    return (box->overlap < negativeBoxOverlapThreshold) && (box->variance > varianceThreshold);
}

void CascadedSingleDetector::init(Frame* frame, Box* box) {
    firstFrame = frame;
    firstBox = box;

    MeanVariance* initialMeanVariance = firstFrame->integral->computeMeanVariance(
        (int) firstBox->x1,
        (int) firstBox->y1,
        (int) firstBox->width,
        (int) firstBox->height
    );
    varianceThreshold = initialMeanVariance->variance * 0.5;

    BoundedPriorityQueue<Box, OverlapOrdered> positiveQueue =
        BoundedPriorityQueue<Box, OverlapOrdered>(nrOfPositiveBoxes4EnsembleAtInitialization);
    BoundedPriorityQueue<Box, OverlapOrdered> negativeQueue =
        BoundedPriorityQueue<Box, OverlapOrdered>(nrOfNegativeBoxes4EnsembleAtInitialization);

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
        frame,
        positiveBoxList4Ensemble,
        negativeBoxList4Ensemble
    );
    eClassifier->init(trainingSet4Ensemble);

    vector<Box*> positiveBoxList4NN = { positiveQueue.head() };
    vector<Box*> negativeBoxList4NN = Random::randomSample(negativeBoxList4Ensemble, nrOfNegativeBoxes4NNAtInitialization);
    TrainingSet<Box> trainingSet4NN = TrainingSet<Box>(
        frame,
        positiveBoxList4NN,
        negativeBoxList4NN
    );

    nnClassifier->init(trainingSet4NN);
}

DetectResult* CascadedSingleDetector::detect(Frame* frame) {
    DetectResult* detectResult = new DetectResult();
    return detectResult;
}

ClassificationDetails* CascadedSingleDetector::score(Frame* frame, Box* box) {
    ClassificationDetails* detail = new ClassificationDetails();
    return detail;
}

void CascadedSingleDetector::learn(Frame* frame, Box* box, DetectResult* detectResult) {
    vector<ScoredBox*> grid = detectResult->all;

    BoundedPriorityQueue<ScoredBox, ScoredBoxOverlapOrdered> positiveQueue =
        BoundedPriorityQueue<ScoredBox, ScoredBoxOverlapOrdered>(10);

    BoundedPriorityQueue<ScoredBox, ScoredBoxOverlapOrdered> negativeQueue =
        BoundedPriorityQueue<ScoredBox, ScoredBoxOverlapOrdered>(INT_MAX);

    BoundedPriorityQueue<ScoredBox, ScoredBoxOverlapOrdered> negativeQueue4NN =
        BoundedPriorityQueue<ScoredBox, ScoredBoxOverlapOrdered>(INT_MAX);

    for (int i = 0; i < detectResult->allSize; i++) {
        ScoredBox* sample = grid[i];

        //Compute Overlap
        Box* sampleBox = sample->box;
        double overlap = Box::computeOverlap(sampleBox, firstBox);
        sampleBox->overlap = overlap;

        if (isPositive(sample->box)) {
            positiveQueue += sample;
            continue;
        }

        if (isNegative(sample->box) && sample->getScore("ensemble") >= 1) {
            negativeQueue += sample;
        }

        if (sample->isDetected && sampleBox->overlap < 0.2) {
            negativeQueue4NN += sample;
        }
    }

    vector<ScoredBox*> positiveBoxList4Ensemble = positiveQueue.toVector();
    vector<ScoredBox*> negativeBoxList4Ensemble = negativeQueue.toVector();
    TrainingSet<ScoredBox> trainingSet4Ensemble = TrainingSet<ScoredBox>(
        frame,
        positiveBoxList4Ensemble,
        negativeBoxList4Ensemble
    );

    eClassifier->update(trainingSet4Ensemble);

    vector<ScoredBox*> positiveBoxList4NN = { positiveQueue.head() };
    vector<ScoredBox*> negativeBoxList4NN = negativeQueue4NN.toVector();
    TrainingSet<ScoredBox> trainingSet4NN = TrainingSet<ScoredBox>(
        frame,
        positiveBoxList4NN,
        negativeBoxList4NN
    );
    nnClassifier->update(trainingSet4NN);
}
