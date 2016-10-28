#include "detector/Detector.hpp"

Detector::Detector(Frame* frame, vector<Box*> boxList) {
    firstFrame = frame;
    firstBox = boxList[0];
    nrOfModels = (int) boxList.size();

    vClassifier = new VarianceClassifier();
    eClassifier = new EnsembleClassifier();
    nnClassifier = new NearestNeighborClassifier();

    maxScaleLimit = 10;
    minimumPatchSize = 24;

    nrOfPositiveBoxes4EnsembleAtInitialization = 10;

    nrOfPositiveBoxes4NNAtInitialization = 1;
    nrOfNegativeBoxes4NNAtInitialization = 50;

    varianceThreshold = 0.0;
    positiveBoxOverlapThreshold = 0.6;
    negativeBoxOverlapThreshold = 0.2;
}

bool Detector::isPositive(Box* box) {
    return (box->overlap > positiveBoxOverlapThreshold);
}

bool Detector::isNegative(Box* box) {
    return (box->overlap < negativeBoxOverlapThreshold) && (box->variance > varianceThreshold);
}

vector<Box*> Detector::init(Frame* frame, vector<Box*> boxList) {

    vClassifier->init(frame, boxList);
    eClassifier->init(frame, boxList);
    nnClassifier->init(frame, boxList);

    int nrOfBoxes = (int) boxList.size();
    vector<Box*> correctedBoxList;

    double minimumVariance = FLT_MAX;
    for (int i = 0; i < nrOfBoxes; i++) {
        Box* box = boxList[i];
        Box* correctedBox = getClosestBox(frame, box);
        double boxVariance = nnClassifier->getPatchVariance(frame, correctedBox);

        vClassifier->setMinimumVariance(i, boxVariance);
        if (boxVariance < minimumVariance) {
            minimumVariance = boxVariance;
        }
    }

    // this->varianceThreshold = minimumVariance / 2.0;
    this->varianceThreshold = 489.4352 / 2.0;

    printf("MINVAR >>> %3.4f\n", this->varianceThreshold);

    for (int i = 0; i < nrOfBoxes; i++) {
            Box* box = boxList[i];
            Box* correctedBox = this->init(frame, box, i);
            correctedBoxList.push_back(correctedBox);
    }
    return correctedBoxList;
}

Box* Detector::init(Frame* frame, Box* box, int modelId) {
    BoundedSortedVector<Box, OverlapOrdered> positiveQueue = BoundedSortedVector<Box, OverlapOrdered>(10);
    vector<Box*> negativeQueue;

    BoxIterator* boxIterator = new BoxIterator(firstFrame, firstBox, maxScaleLimit, minimumPatchSize);
    while (boxIterator->hasNext()) {
        Box* sampleBox = boxIterator->next();

        printf("%s\n", sampleBox->toCharArr());

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
            negativeQueue.push_back(sampleBox);
            continue;
        }
    }

    vector<Box*> positiveBoxList4Ensemble = positiveQueue.toVector();
    vector<ScoredBox*> positiveScoredBoxList4Ensemble = score(frame, positiveBoxList4Ensemble);
    vector<ScoredBox*> negativeScoredBoxList4Ensemble = score(frame, negativeQueue);

    Random::seed();
    vector<ScoredBox*> negativeScoredBoxList4EnsembleFirstPart = Random::splitData(negativeScoredBoxList4Ensemble, 2);

    printf("====== TRAINING DATA FOR EC ======\n");
    for (int i = 0; i < (int) positiveScoredBoxList4Ensemble.size(); i++) {
        ScoredBox* scoredBox = positiveScoredBoxList4Ensemble[i];
        printf(COLOR_GREEN "%s\n" COLOR_RESET, scoredBox->box->toCharArr());
    }

    for (int i = 0; i < (int) negativeScoredBoxList4Ensemble.size(); i++) {
        ScoredBox* scoredBox = negativeScoredBoxList4Ensemble[i];
        printf(COLOR_RED "%s\n" COLOR_RESET, scoredBox->box->toCharArr());
    }
    printf("==================================\n");

    Box* closestBox = positiveScoredBoxList4Ensemble[0]->box;
    vector<ScoredBox*> positiveScoredBoxList4NN = { positiveScoredBoxList4Ensemble[0] };

    Random::seed();
    vector<ScoredBox*> negativeScoredBoxList4NN = Random::randomSample(
        negativeScoredBoxList4Ensemble,
        100
    );

    Random::seed();
    vector<ScoredBox*> negativeScoredBoxList4NNFirstPart = Random::splitData(negativeScoredBoxList4NN, 2);

    printf("====== TRAINING DATA FOR NN ======\n");
    for (int i = 0; i < (int) positiveScoredBoxList4NN.size(); i++) {
        ScoredBox* scoredBox = positiveScoredBoxList4NN[i];
        printf(COLOR_GREEN "%s\n" COLOR_RESET, scoredBox->box->toCharArr());
    }

    for (int i = 0; i < (int) negativeScoredBoxList4NNFirstPart.size(); i++) {
        ScoredBox* scoredBox = negativeScoredBoxList4NNFirstPart[i];
        printf(COLOR_RED "%s\n" COLOR_RESET, scoredBox->box->toCharArr());
    }
    printf("==================================\n");

    TrainingSet<ScoredBox> trainingSet4Ensemble = TrainingSet<ScoredBox>(
        frame,
        positiveScoredBoxList4Ensemble,
        negativeScoredBoxList4EnsembleFirstPart,
        2
    );

    TrainingSet<ScoredBox> trainingSet4NN = TrainingSet<ScoredBox>(
        frame,
        positiveScoredBoxList4NN,
        negativeScoredBoxList4NNFirstPart,
        1
    );

    eClassifier->train(trainingSet4Ensemble, modelId);
    nnClassifier->train(trainingSet4NN, modelId);

    return closestBox;
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

void Detector::learn(Frame* current, Box* box, vector<ScoredBox*> grids, int modelId) {
    int gridSize = (int) grids.size();

    BoundedSortedVector<ScoredBox, ScoredBoxOverlapOrdered> positiveQueue = BoundedSortedVector<ScoredBox, ScoredBoxOverlapOrdered>(10);
    vector<ScoredBox*> negativeQueue;
    vector<ScoredBox*> negativeQueue4NN;

    int nrOfPositive = 0;
    int nrOfNegativeEC = 0;

    for (int i = 0; i < gridSize; i++) {
        ScoredBox* sample = grids[i];

        //Compute Overlap
        Box* sampleBox = sample->box;
        double overlap = Box::computeOverlap(sampleBox, box);
        sampleBox->overlap = overlap;

        if (isPositive(sample->box)) {
            nrOfPositive += 1;
            score(current, sample);
            positiveQueue += sample;
            continue;
        }

        EnsembleScore* ensembleScore = (EnsembleScore*) sample->getScore("ensemble");
        double confidence = 0.0;
        if (ensembleScore != nullptr) {
            confidence = ensembleScore->scores[modelId];
        }

        if (isNegative(sample->box) && confidence >= 1.0) {
            nrOfNegativeEC += 1;
            score(current, sample);
            negativeQueue.push_back(sample);
        }

        //TOCHECK: It might use the boxes classified by EnsembleClassifier.
        bool isClassified = sample->isClassified("ensemble", modelId);
        if (isClassified && sampleBox->overlap < 0.2) {
            score(current, sample);
            negativeQueue4NN.push_back(sample);
        }
    }

    vector<ScoredBox*> positiveScoredBoxList4Ensemble = positiveQueue.toVector();
    vector<ScoredBox*> negativeScoredBoxList4Ensemble = negativeQueue;

    printf("====== TRAINING DATA FOR EC ======\n");
    printf("Positive Boxes: %d / %d\n", (int) positiveScoredBoxList4Ensemble.size(), nrOfPositive);
    for (int i = 0; i < (int) positiveScoredBoxList4Ensemble.size(); i++) {
        ScoredBox* scoredBox = positiveScoredBoxList4Ensemble[i];
        printf(COLOR_GREEN "%s - %f\n" COLOR_RESET, scoredBox->box->toCharArr(), scoredBox->box->overlap);
    }

    printf("Negative Boxes: %d\n", (int) negativeScoredBoxList4Ensemble.size());
    for (int i = 0; i < (int) negativeScoredBoxList4Ensemble.size(); i++) {
        ScoredBox* scoredBox = negativeScoredBoxList4Ensemble[i];
        printf(COLOR_RED "%s\n" COLOR_RESET, scoredBox->box->toCharArr());
    }
    printf("==================================\n");

    vector<ScoredBox*> positiveScoredBoxList4NN;
    if (positiveScoredBoxList4Ensemble.size() > 0) {
        positiveScoredBoxList4NN = { positiveScoredBoxList4Ensemble[0] };
    }
    vector<ScoredBox*> negativeScoredBoxList4NN = negativeQueue4NN;

    printf("====== TRAINING DATA FOR NN ======\n");
    printf("Positive Boxes: %d\n", (int) positiveScoredBoxList4NN.size());
    for (int i = 0; i < (int) positiveScoredBoxList4NN.size(); i++) {
        ScoredBox* scoredBox = positiveScoredBoxList4NN[i];
        printf(COLOR_GREEN "%s\n" COLOR_RESET, scoredBox->box->toCharArr());
    }

    printf("Negative Boxes: %d\n", (int) negativeScoredBoxList4NN.size());
    for (int i = 0; i < (int) negativeScoredBoxList4NN.size(); i++) {
        ScoredBox* scoredBox = negativeScoredBoxList4NN[i];
        printf(COLOR_RED "%s\n" COLOR_RESET, scoredBox->box->toCharArr());
    }
    printf("==================================\n");

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

    eClassifier->train(trainingSet4Ensemble, modelId);
    nnClassifier->train(trainingSet4NN, modelId);
}

vector<ScoredBox*> Detector::detect(Frame* frame) {
    vector<ScoredBox*> allBoxList;
    BoxIterator* iterator = new BoxIterator(frame, firstBox, maxScaleLimit, minimumPatchSize);

    while (iterator->hasNext()) {
        Box* nextBox = iterator->next();
        ScoredBox* scoredBox = new ScoredBox(nextBox);
        allBoxList.push_back(scoredBox);

        if (!vClassifier->classify(frame, scoredBox)) {
            continue;
        }

        if (!eClassifier->classify(frame, scoredBox)) {
            continue;
        }

        printf("EC >> %s\n", nextBox->toCharArr());

        if (!nnClassifier->classify(frame, scoredBox)) {
            continue;
        }

        NNScore* nnScore = (NNScore*) scoredBox->getScore("nn");
        double relativeScore = nnScore->relativeScores[0];
        double conservativeValue = nnScore->conservativeScores[0];
        printf("NN >> %s, %g, %g\n", nextBox->toCharArr(), relativeScore, conservativeValue);

        scoredBox->isDetected = true;
    }
    return allBoxList;
}

ScoredBox* Detector::validate(Frame* frame, Box* box, int modelId) {
    ScoredBox* scoredBox = new ScoredBox(box);
    nnClassifier->validate(frame, scoredBox, modelId);
    return scoredBox;
}

bool Detector::evaluate(Frame* frame, Box* box, double minVariance, int modelId) {
    return nnClassifier->evaluate(frame, box, minVariance, modelId);
}

void Detector::dumpDetector() {
    eClassifier->dumpEnsembleClassifier();
    nnClassifier->dumpNearestNeighborClassifier();
}

Box* Detector::getClosestBox(Frame* frame, Box* box) {
    BoxIterator* boxIterator = new BoxIterator(firstFrame, firstBox, maxScaleLimit, minimumPatchSize);
    double minimumOverlap = 0.0;
    Box* closestBox = (Box*) nullptr;
    while (boxIterator->hasNext()) {
        Box* nextBox = boxIterator->next();
        double overlap = Box::computeOverlap(nextBox, box);

        if (overlap > minimumOverlap) {
            minimumOverlap = overlap;
            closestBox = nextBox;
        }
    }
    return closestBox;
}
