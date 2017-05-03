#include "detector/Detector.hpp"
#include <iostream>
#include <fstream>

Detector::Detector() {
    eClassifier = new EnsembleClassifier();
    nnClassifier = new NearestNeighborClassifier();

    maxScaleLimit = 10;
    minimumPatchSize = 24;

    nrOfPositiveBoxes4EnsembleAtInitialization = 10;
    nrOfPositiveBoxes4EnsembleAtUpdate = 10;

    nrOfPositiveBoxes4NNAtInitialization = 1;
    nrOfNegativeBoxes4NNAtInitialization = 100;

    positiveBoxOverlapThreshold = 0.6;
    negativeBoxOverlapThreshold = 0.2;

    maxOverlapOfNegativeSampleForNN = 0.2;
}

Detector::~Detector() {
    delete eClassifier;
    delete nnClassifier;
}

bool Detector::isPositive(Box* box) {
    return (box->overlap > positiveBoxOverlapThreshold);
}

bool Detector::isNegative(Box* box, double varianceThresholdPerModel) {
    return (box->overlap < negativeBoxOverlapThreshold) && (box->variance > varianceThresholdPerModel);
}

vector<Box*> Detector::init(Frame* frame, vector<Box*> boxList) {
    Random::seed();

    //TODO Assuming all boxes are in the same scale
    firstBox = boxList[0];
    nrOfModels = (int) boxList.size();
    initVarianceThresholds(frame, boxList);

    eClassifier->init(frame, boxList);
    printf("mc2000\n");
    nnClassifier->init(frame, boxList);
    printf("mc2100\n");

    int nrOfBoxes = (int) boxList.size();
    printf("mc2200\n");

    vector<Box*> correctedBoxList;

    for (int i = 0; i < nrOfBoxes; i++) {
        printf("Init: %d\n", i);
        Box* box = boxList[i];
        Box* correctedBox = this->init(frame, box, i);
        correctedBoxList.push_back(correctedBox);
    }
    return correctedBoxList;
}

void Detector::initVarianceThresholds(Frame* frame, vector<Box*> boxList) {
    this->varianceList.resize(nrOfModels);

    double minimumVariance = FLT_MAX;
    for (int i = 0; i < nrOfModels; i++) {
        int modelId = i;

        Box* box = boxList[i];
        Box* correctedBox = getClosestBox(frame, box);
        double boxVariance = nnClassifier->getPatchVariance(frame, correctedBox);
        boxVariance = boxVariance / 2.0;

        this->varianceList[modelId] = boxVariance;
        if (boxVariance < minimumVariance) {
            minimumVariance = boxVariance;
        }

        delete correctedBox;
    }

    this->minimumVariance = minimumVariance;
}

Box* Detector::init(Frame* frame, Box* box, int modelId) {
    BoundedSortedVector<Box, OverlapOrdered> positiveQueue =
        BoundedSortedVector<Box, OverlapOrdered>(nrOfPositiveBoxes4EnsembleAtInitialization);
    vector<Box*> negativeQueue;

    BoxIterator* boxIterator = new BoxIterator(frame, firstBox);
    int id = 0;

    double varianceThresholdForModel = varianceList[modelId] / 2.0;

    while (boxIterator->hasNext()) {
        Box* sampleBox = boxIterator->next();
        id += 1;

        //Compute Overlap
        double overlap = Box::computeOverlap(sampleBox, box);
        sampleBox->overlap = overlap;

        //Compute Variance
        MeanVariance* meanVariance = frame->integralImage->computeMeanVariance(
            (int) sampleBox->x1,
            (int) sampleBox->y1,
            (int) sampleBox->width,
            (int) sampleBox->height
        );

        sampleBox->mean = meanVariance->mean;
        sampleBox->variance = meanVariance->variance;

        delete meanVariance;

        //Check if positive
        if (isPositive(sampleBox)) {
            positiveQueue += sampleBox;
            continue;
        }

        //Check if negative
        if (isNegative(sampleBox, varianceThresholdForModel)) {
            // Add to TrainingSetBuilder as positive
            negativeQueue.push_back(sampleBox);
            continue;
        }

        delete sampleBox;
    }

    delete boxIterator;

    vector<Box*> positiveBoxList4Ensemble = positiveQueue.toVector();

    Box* closestBox = positiveBoxList4Ensemble[0]->clone();
    vector<Box*> positiveScoredBoxList4NN = { closestBox };

    Random::seed();
    vector<Box*> negativeBoxList4NN = Random::randomSample(
        negativeQueue,
        nrOfNegativeBoxes4NNAtInitialization
    );

    Random::seed();
    vector<Box*> negativeBoxList4EnsembleFirstPart = Random::splitData(negativeQueue, 2);

    Random::seed();
    vector<Box*> negativeBoxList4NNFirstPart = Random::splitData(negativeBoxList4NN, 2);

    TrainingSet<Box> trainingSet4Ensemble = TrainingSet<Box>(
        frame,
        positiveBoxList4Ensemble,
        negativeBoxList4EnsembleFirstPart,
        2
    );

    TrainingSet<Box> trainingSet4NN = TrainingSet<Box>(
        frame,
        positiveScoredBoxList4NN,
        negativeBoxList4NNFirstPart,
        1
    );

    eClassifier->train(trainingSet4Ensemble, modelId, 0.0);
    nnClassifier->train(trainingSet4NN, modelId);

    int nrOfPositiveSamples = (int) positiveBoxList4Ensemble.size();
    for (int i = 0; i < nrOfPositiveSamples; i++) {
        Box* box = positiveBoxList4Ensemble[i];
        delete box;
    }

    int nrOfNegativeSamples = (int) negativeQueue.size();
    for (int i = 0; i < nrOfNegativeSamples; i++) {
        Box* box = negativeQueue[i];
        delete box;
    }

    return closestBox;
}

void Detector::learn(Frame* current, Box* box, vector<ScoredBox*> grids, int modelId) {
    int gridSize = (int) grids.size();

    BoundedSortedVector<ScoredBox, ScoredBoxOverlapOrdered> positiveQueue =
        BoundedSortedVector<ScoredBox, ScoredBoxOverlapOrdered>(nrOfPositiveBoxes4EnsembleAtUpdate);
    vector<ScoredBox*> negativeQueue;
    vector<ScoredBox*> negativeQueue4NN;

    int nrOfPositive = 0;
    int nrOfNegativeEC = 0;

    double varianceThresholdForModel = varianceList[modelId];

    for (int i = 0; i < gridSize; i++) {
        ScoredBox* sample = grids[i];

        //Compute Overlap
        Box* sampleBox = sample->box;
        double overlap = Box::computeOverlap(sampleBox, box);
        sampleBox->overlap = overlap;

        if (isPositive(sample->box)) {
            nrOfPositive += 1;
            score(current, sample);
            positiveQueue.append(sample);
            continue;
        }

        EnsembleScore* ensembleScore = (EnsembleScore*) sample->getScore("ensemble");
        double confidence = 0.0;
        if (ensembleScore != nullptr) {
            confidence = ensembleScore->scores[modelId];
        }

        if (isNegative(sample->box, varianceThresholdForModel) && confidence >= 1.0) {
            nrOfNegativeEC += 1;
            score(current, sample);
            negativeQueue.push_back(sample);
        }

        //TOCHECK: It might use the boxes classified by EnsembleClassifier.
        bool isClassified = sample->isClassified("ensemble", modelId);
        if (isClassified && sampleBox->overlap < maxOverlapOfNegativeSampleForNN) {
            score(current, sample);
            negativeQueue4NN.push_back(sample);
        }
    }

    vector<ScoredBox*> positiveScoredBoxList4Ensemble = positiveQueue.toVector();
    vector<ScoredBox*> negativeScoredBoxList4Ensemble = negativeQueue;

    vector<ScoredBox*> positiveScoredBoxList4NN;
    if (positiveScoredBoxList4Ensemble.size() > 0) {
        positiveScoredBoxList4NN = { positiveScoredBoxList4Ensemble[0] };
    }

    vector<ScoredBox*> negativeScoredBoxList4NN = negativeQueue4NN;
    TrainingSet<ScoredBox> trainingSet4Ensemble = TrainingSet<ScoredBox>(
        current,
        positiveScoredBoxList4Ensemble,
        negativeScoredBoxList4Ensemble,
        2,
        false
    );

    TrainingSet<ScoredBox> trainingSet4NN = TrainingSet<ScoredBox>(
        current,
        positiveScoredBoxList4NN,
        negativeScoredBoxList4NN
    );

    eClassifier->train(trainingSet4Ensemble, modelId, 0.0);
    nnClassifier->train(trainingSet4NN, modelId);
}

vector<ScoredBox*> Detector::score(Frame* frame, vector<Box*> boxList) {
    vector<ScoredBox*> scoredBoxList;

    int nrOfBoxes = (int) boxList.size();
    for (int i = 0; i < nrOfBoxes; i++) {
        Box* box = boxList[i];
        ScoredBox* scoredBox = new ScoredBox(box);
        score(frame, scoredBox);
        scoredBoxList.push_back(scoredBox);
    }

    return scoredBoxList;
}

void Detector::score(Frame* frame, ScoredBox* scoredBox) {
    eClassifier->score(frame, scoredBox);
    nnClassifier->score(frame, scoredBox);
}

bool Detector::checkVariance(Frame* frame, ScoredBox* scoredBox) {
    Box* box = scoredBox->box;
    MeanVariance* meanVariance = frame->integralImage->computeMeanVariance(
        (int) box->x1,
        (int) box->y1,
        (int) box->width,
        (int) box->height);

    box->mean = meanVariance->mean;
    box->variance = meanVariance->variance;

    delete meanVariance;

    VarianceScore* score = new VarianceScore();
    for (int i = 0; i < this->nrOfModels; i++) {
        double variance = this->varianceList[i];
        if (box->variance >= variance) {
            score->classifiedModelIds.push_back(i);
        }
    }


    score->isAnyModellClassified = box->variance >= minimumVariance;
    scoredBox->withScore("variance", score);

    return score->isAnyModellClassified;
}

vector<ScoredBox*> Detector::detect(Frame* frame) {
    vector<ScoredBox*> allBoxList;
    BoxIterator* iterator = new BoxIterator(frame, firstBox);

    while (iterator->hasNext()) {
        Box* nextBox = iterator->next();
        ScoredBox* scoredBox = new ScoredBox(nextBox);
        allBoxList.push_back(scoredBox);

        if (!this->checkVariance(frame, scoredBox)) {
            continue;
        }

        if (!eClassifier->classify(frame, scoredBox)) {
            continue;
        }

        if (!nnClassifier->classify(frame, scoredBox)) {
            continue;
        }

        scoredBox->isDetected = true;
    }

    delete iterator;
    return allBoxList;
}

ScoredBox* Detector::validate(Frame* frame, Box* box, int modelId) {
    ScoredBox* scoredBox = new ScoredBox(box);
    nnClassifier->validate(frame, scoredBox, modelId);
    return scoredBox;
}

bool Detector::evaluate(Frame* frame, Box* box, int modelId) {
    double varianceThresholdForModel = varianceList[modelId] / 2.0;
    bool evaluationResult = nnClassifier->evaluate(frame, box, varianceThresholdForModel, modelId);
    return evaluationResult;
}

Box* Detector::getClosestBox(Frame* frame, Box* box) {
    BoxIterator* boxIterator = new BoxIterator(frame, firstBox);
    double minimumOverlap = 0.0;
    Box* closestBox = (Box*) nullptr;
    while (boxIterator->hasNext()) {
        Box* nextBox = boxIterator->next();
        double overlap = Box::computeOverlap(nextBox, box);

        if (overlap > minimumOverlap) {
            minimumOverlap = overlap;
            delete closestBox;
            closestBox = nextBox;
        } else {
            delete nextBox;
        }
    }

    delete boxIterator;
    return closestBox;
}
