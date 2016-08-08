#include "detector/Detector.hpp"

Detector::Detector(Frame* frame, vector<Box*> boxList) {
    firstFrame = frame;
    firstBox = boxList[0];

    vClassifier = new VarianceClassifier(frame, boxList);
    eClassifier = new EnsembleClassifier();
    nnClassifier = new NearestNeighborClassifier();

    maxScaleLimit = 10;
    minimumPatchSize = 24;

    nrOfPositiveBoxes4EnsembleAtInitialization = 10;
    nrOfNegativeBoxes4EnsembleAtInitialization = 100;

    nrOfPositiveBoxes4NNAtInitialization = 1;
    nrOfNegativeBoxes4NNAtInitialization = 10;

    positiveBoxOverlapThreshold = 0.6;
    negativeBoxOverlapThreshold = 0.2;
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

vector<ScoredBox*> Detector::detect(Frame* frame) {
    vector<ScoredBox*> allBoxList;
    BoxIterator* iterator = new BoxIterator(frame, firstBox, maxScaleLimit, minimumPatchSize);

    int allPass = 0;
    int variancePass = 0;
    int ensemblePass = 0;

    int nearestNeighborPass = 0;

    while (iterator->hasNext()) {
        Box* nextBox = iterator->next();
        allPass += 1;
        if (!vClassifier->classify(frame, nextBox)) {
            continue;
        }
        variancePass += 1;

        ScoredBox* scoredBox = new ScoredBox(nextBox);
        allBoxList.push_back(scoredBox);
        if (!eClassifier->classify(frame, scoredBox)) {
            continue;
        }
        ensemblePass += 1;

        ImageBuilder* builder = new ImageBuilder();
        builder->
            withFrame(frame)->
            withBox(nextBox)
            ->display();


        if (!nnClassifier->classify(frame, scoredBox)) {
            continue;
        }
        printf("NEARESTN >>> %6d\n", nearestNeighborPass);
        nearestNeighborPass += 1;
    }


    printf("VARIANCE >>> %6d\n", variancePass);
    printf("ENSEMBLE >>> %6d\n", ensemblePass);
    printf("NEARESTN >>> %6d\n", nearestNeighborPass);
    printf("---------------------\n");
    printf("ALLPATCH >>> %6d\n", allPass);

    return allBoxList;
}
