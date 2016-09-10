#include "detector/Detector.hpp"

Detector::Detector(Frame* frame, vector<Box*> boxList) {
    firstFrame = frame;
    firstBox = boxList[0];

    vClassifier = new VarianceClassifier(frame, boxList);
    eClassifier = new EnsembleClassifier(frame, boxList);
    nnClassifier = new NearestNeighborClassifier(frame, boxList);

    maxScaleLimit = 10;
    minimumPatchSize = 24;

    nrOfPositiveBoxes4EnsembleAtInitialization = 10;
    nrOfNegativeBoxes4EnsembleAtInitialization = 100;

    nrOfPositiveBoxes4NNAtInitialization = 1;
    nrOfNegativeBoxes4NNAtInitialization = 10;

    positiveBoxOverlapThreshold = 0.6;
    negativeBoxOverlapThreshold = 0.2;
    nrOfModels = (int) boxList.size();
}

bool Detector::isPositive(Box* box) {
    return (box->overlap > positiveBoxOverlapThreshold);
}

bool Detector::isNegative(Box* box) {
    return (box->overlap < negativeBoxOverlapThreshold) && (box->variance > varianceThreshold);
}

void Detector::init(Frame* frame, vector<Box*> boxList) {
    int nrOfBoxes = (int) boxList.size();
    for (int i = 0; i < nrOfBoxes; i++) {
            Box* box = boxList[i];
            this->init(frame, box, i);
    }
}

void Detector::init(Frame* frame, Box* box, int modelId) {
    BoundedPriorityQueue<Box, OverlapOrdered> positiveQueue =
        BoundedPriorityQueue<Box, OverlapOrdered>(nrOfPositiveBoxes4EnsembleAtInitialization);
    BoundedPriorityQueue<Box, OverlapOrdered> negativeQueue =
        BoundedPriorityQueue<Box, OverlapOrdered>(nrOfNegativeBoxes4EnsembleAtInitialization);

    BoxIterator* boxIterator = new BoxIterator(firstFrame, firstBox, maxScaleLimit, minimumPatchSize);
    while (boxIterator->hasNext()) {
        Box* sampleBox = boxIterator->next();

        //Compute Overlap
        double overlap = Box::computeOverlap(sampleBox, box);
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

    eClassifier->train(trainingSet4Ensemble, modelId);

    vector<Box*> positiveBoxList4NN = { positiveQueue.head() };
    vector<Box*> negativeBoxList4NN = Random::randomSample(
        negativeBoxList4Ensemble,
        nrOfNegativeBoxes4NNAtInitialization
    );
    TrainingSet<Box> trainingSet4NN = TrainingSet<Box>(
        frame,
        positiveBoxList4NN,
        negativeBoxList4NN
    );

    nnClassifier->train(trainingSet4NN, modelId);
}

void Detector::learn(Frame* current,
                     Box* box,
                     vector<ScoredBox*> grids,
                     int modelId) {
    int gridSize = (int) grids.size();
    BoundedPriorityQueue<ScoredBox, ScoredBoxOverlapOrdered> positiveQueue =
        BoundedPriorityQueue<ScoredBox, ScoredBoxOverlapOrdered>(10);

    BoundedPriorityQueue<ScoredBox, ScoredBoxOverlapOrdered> negativeQueue =
        BoundedPriorityQueue<ScoredBox, ScoredBoxOverlapOrdered>(INT_MAX);

    BoundedPriorityQueue<ScoredBox, ScoredBoxOverlapOrdered> negativeQueue4NN =
        BoundedPriorityQueue<ScoredBox, ScoredBoxOverlapOrdered>(INT_MAX);

    for (int i = 0; i < gridSize; i++) {
        ScoredBox* sample = grids[i];

        //Compute Overlap
        Box* sampleBox = sample->box;
        double overlap = Box::computeOverlap(sampleBox, box);

        sampleBox->overlap = overlap;

        if (isPositive(sample->box)) {
            positiveQueue += sample;
            continue;
        }

        if (isNegative(sample->box) && sample->getScoreValue("ensemble") >= 1) {
            negativeQueue += sample;
        }

        //TOCHECK: It might use the boxes classified by EnsembleClassifier.
        if (sample->isDetected && sampleBox->overlap < 0.2) {
            negativeQueue4NN += sample;
        }

    }

    vector<ScoredBox*> positiveBoxList4Ensemble = positiveQueue.toVector();
    vector<ScoredBox*> negativeBoxList4Ensemble = negativeQueue.toVector();
    TrainingSet<ScoredBox> trainingSet4Ensemble = TrainingSet<ScoredBox>(
        current,
        positiveBoxList4Ensemble,
        negativeBoxList4Ensemble
    );

    eClassifier->train(trainingSet4Ensemble, modelId);

    vector<ScoredBox*> positiveBoxList4NN;
    if (positiveQueue.count > 0) {
        positiveBoxList4NN = { positiveQueue.head() };
    }
    vector<ScoredBox*> negativeBoxList4NN = negativeQueue4NN.toVector();

    TrainingSet<ScoredBox> trainingSet4NN = TrainingSet<ScoredBox>(
        current,
        positiveBoxList4NN,
        negativeBoxList4NN
    );
    nnClassifier->train(trainingSet4NN, modelId);
}

vector<ScoredBox*> Detector::detect(Frame* frame) {
    vector<ScoredBox*> allBoxList;
    BoxIterator* iterator = new BoxIterator(frame, firstBox, maxScaleLimit, minimumPatchSize);

    while (iterator->hasNext()) {
        Box* nextBox = iterator->next();
        ScoredBox* scoredBox = new ScoredBox(nextBox);

        if (!vClassifier->classify(frame, scoredBox)) {
            continue;
        }
        allBoxList.push_back(scoredBox);

        if (!eClassifier->classify(frame, scoredBox)) {
            continue;
        }

        if (!nnClassifier->classify(frame, scoredBox)) {
            continue;
        }

        scoredBox->isDetected = true;
    }
    return allBoxList;
}

ScoredBox* Detector::validate(Frame* frame, Box* box, int modelId) {
    ScoredBox* scoredBox = new ScoredBox(box);
    nnClassifier->validate(frame, scoredBox, modelId);
    return scoredBox;
}
