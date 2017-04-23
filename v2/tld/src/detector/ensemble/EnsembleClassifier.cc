#include "detector/ensemble/EnsembleClassifier.hpp"

EnsembleClassifier::EnsembleClassifier() {
    this->initiated = false;
    this->classifierName = "ensemble";

    this->nrOfBaseClassifiers = 10;
    this->nrOfPixelComparisons = 13;

    this->positiveLabelThreshold = 0.5 * nrOfBaseClassifiers;
    this->positiveUpdateThreshold = 0.5 * nrOfBaseClassifiers;
    this->negativeUpdateThreshold = 0.5 * nrOfBaseClassifiers;
}

EnsembleClassifier::~EnsembleClassifier() {
    if (this->initiated) {
        for (int i = 0; i < nrOfBaseClassifiers; i++) {
            BaseClassifier* bc = baseClassifiers[i];
            delete bc;
        }
    }
}

//Initialization Logic

void EnsembleClassifier::init(Frame* firstFrame, vector<Box*> boxList) {
    this->initiated = true;
    this->nrOfModels = (int) boxList.size();
    this->baseClassifiers = generateBaseClassifier();
}

vector<BaseClassifier*> EnsembleClassifier::generateBaseClassifier() {
    Random::seed();
    vector<PixelComparison*> allComparisons = produceAllComparisons();
    printf("Number of all comparisons: %d\n", (int) allComparisons.size());

    vector<BaseClassifier*> classifiers = shuffleComparisons(allComparisons);
    printf("Ensemble Classifier is created with %lu base classifiers\n", classifiers.size());
    return classifiers;
}

Point2f shift(Point2f& point, double x, double y) {
    Point2f shiftedPoint = Point2f(point.x + x, point.y + y);
    return shiftedPoint;
}

Point2f shift(Point2f& point, int direction) {
    Point2f shiftedPoint;
    double random = (double) Random::randomFloat();
    switch (direction) {
        case POS_X_DIR:
            /* code */
            shiftedPoint = shift(point, random + 0.2, 0.0);
            break;
        case NEG_X_DIR:
            /* code */
            shiftedPoint = shift(point, -(random + 0.2), 0.0);
            break;
        case POS_Y_DIR:
            /* code */
            shiftedPoint = shift(point, 0.0, random + 0.2);
            break;
        case NEG_Y_DIR:
            /* code */
            shiftedPoint = shift(point, 0.0, -(random + 0.2));
            break;
        default:
            /* code */
            shiftedPoint = point;
            break;
    }
    return shiftedPoint;
}

vector<Point2f> shiftPoints(vector<Point2f> points, int size, int direction) {
    vector<Point2f> shiftedPoints;
    for (int i = 0; i < size; i++) {
        Point2f point = points[i];
        Point2f shiftedPoint = shift(point, direction);
        shiftedPoints.push_back(shiftedPoint);
    }
    return shiftedPoints;
}

vector<PixelComparison*> EnsembleClassifier::produceAllComparisons() {
    vector<Point2f> fromList;
    for (float i = 0.0; i <= 1.0; i = i + 0.2) {
        for (float j = 0.0; j <= 1.0; j = j + 0.2) {
            fromList.push_back(Point2f(i, j));
        }
    }

    int nrOfPoints = (int) fromList.size();
    for (int i = 0; i < nrOfPoints; i++) {
        Point2f fromPoint = fromList[i];
        Point2f newFromPoint = Point2f(fromPoint.x + 0.1, fromPoint.y + 0.1);
        fromList.push_back(newFromPoint);
    }

    nrOfPoints *= 2;
    vector<Point2f> toList;
    toList.insert(toList.end(), fromList.begin(), fromList.end());

    // printf("------- R ------\n");
    // double random = (double) Random::randomFloat();
    vector<Point2f> rightShiftedList = shiftPoints(toList, nrOfPoints, POS_X_DIR);

    // printf("------- L ------\n");
    // random = (double) Random::randomFloat();
    vector<Point2f> leftShiftedList = shiftPoints(toList, nrOfPoints, NEG_X_DIR);

    // printf("------- T ------\n");
    // random = (double) Random::randomFloat();
    vector<Point2f> topShiftedList = shiftPoints(toList, nrOfPoints, NEG_Y_DIR);

    // printf("------- B ------\n");
    // random = (double) Random::randomFloat();
    vector<Point2f> bottomShiftedList = shiftPoints(toList, nrOfPoints, POS_Y_DIR);

    vector<Point2f> finalFromList;
    vector<Point2f> finalToList;

    for (int i = 0; i < nrOfPoints; i++) {
        Point2f from = fromList[i];
        Point2f rightShiftedPoint = rightShiftedList[i];
        if (from.x > 0 && from.x < 1 && from.y > 0 && from.y < 1) {
            finalFromList.push_back(from);
            finalToList.push_back(rightShiftedPoint);
        }
    }

    for (int i = 0; i < nrOfPoints; i++) {
        Point2f from = fromList[i];
        Point2f leftShiftedPoint = leftShiftedList[i];
        if (from.x > 0 && from.x < 1 && from.y > 0 && from.y < 1) {
            finalFromList.push_back(from);
            finalToList.push_back(leftShiftedPoint);
        }
    }

    for (int i = 0; i < nrOfPoints; i++) {
        Point2f from = fromList[i];
        Point2f topShiftedPoint = topShiftedList[i];
        if (from.x > 0 && from.x < 1 && from.y > 0 && from.y < 1) {
            finalFromList.push_back(from);
            finalToList.push_back(topShiftedPoint);
        }
    }

    for (int i = 0; i < nrOfPoints; i++) {
        Point2f from = fromList[i];
        Point2f bottomShiftedPoint = bottomShiftedList[i];
        if (from.x > 0 && from.x < 1 && from.y > 0 && from.y < 1) {
            finalFromList.push_back(from);
            finalToList.push_back(bottomShiftedPoint);
        }
    }

    nrOfPoints = (int) finalFromList.size();
    vector<PixelComparison*> allComparisons(nrOfPoints);
    for (int i = 0; i < nrOfPoints; i++) {
        Point2f* fromPoint = new Point2f(finalFromList[i]);
        Point2f* toPoint = new Point2f(finalToList[i]);

        if (fromPoint->x < 0) fromPoint->x = 0;
        if (fromPoint->x > 1) fromPoint->x = 1;

        if (fromPoint->y < 0) fromPoint->y = 0;
        if (fromPoint->y > 1) fromPoint->y = 1;

        if (toPoint->x < 0) toPoint->x = 0;
        if (toPoint->x > 1) toPoint->x = 1;

        if (toPoint->y < 0) toPoint->y = 0;
        if (toPoint->y > 1) toPoint->y = 1;

        PixelComparison* comparison = new PixelComparison(fromPoint, toPoint);
        allComparisons[i] = comparison;
    }
    return allComparisons;
}

vector<BaseClassifier*> EnsembleClassifier::shuffleComparisons(vector<PixelComparison*> allComparisons) {
    allComparisons = Random::randomSample(allComparisons);

    vector<BaseClassifier*> classifiers;
    int usedComparisons = 0;
    for (int i = 0; i < nrOfBaseClassifiers; i++) {
        vector<PixelComparison*> comparisons;
        for (int j = 0; j < nrOfPixelComparisons; j++) {
            usedComparisons += 1;
            PixelComparison* pc = allComparisons[i*nrOfPixelComparisons+j];
            comparisons.push_back(pc);
        }
        BaseClassifier* classifier = new BaseClassifier(i, comparisons, this->nrOfModels);
        classifiers.push_back(classifier);
    }

    int remainingIndex = nrOfBaseClassifiers * nrOfPixelComparisons;
    int allComparisonSize = (int) allComparisons.size();
    for (int i = remainingIndex; i < allComparisonSize; i++) {
        PixelComparison* comp = allComparisons[i];
        delete comp;
    }
    return classifiers;
}

//Core Logic

CodeVector* EnsembleClassifier::generateBinaryCode(Frame* frame, Box* box) {
    CodeVector* codeVector = new CodeVector(nrOfBaseClassifiers);
    for (int i = 0; i < nrOfBaseClassifiers; i++) {
        BaseClassifier* bc = baseClassifiers[i];
        int binaryCode = bc->generateBinaryCode(frame, box);
        codeVector->set(i, binaryCode);
    }
    return codeVector;
}

double EnsembleClassifier::getProbability(CodeVector* codeVector, int modelId) {
    double probability = 0.0;
    for (int i = 0; i < nrOfBaseClassifiers; i++) {
        BaseClassifier* bc = baseClassifiers[i];
        int binaryCode = codeVector->get(i);

        probability += bc->getProbability(binaryCode, modelId);
    }
    return probability;
}

//Training Logic

void EnsembleClassifier::train(TrainingSet<Box> ts, int modelId, double varianceThreshold) {
    Frame* frame = ts.frame;
    vector<Box*> positiveBoxList = ts.positiveSamples;
    vector<Box*> negativeBoxList = ts.negativeSamples;

    vector<Labelled<CodeVector>*> samples = generateSamples(frame, positiveBoxList, negativeBoxList, varianceThreshold);

    Random::seed();
    vector<Labelled<CodeVector>*> shuffledSamples = Random::randomSample(samples);

    doTrain(shuffledSamples, modelId);
}

vector<Labelled<CodeVector>*> EnsembleClassifier::generateSamples(
    Frame* frame,
    vector<Box*> positiveBoxList,
    vector<Box*> negativeBoxList,
    double varianceThreshold) {
        double angle = 20.0;
        double scale = 0.02;
        double shift = 0.02;

        vector<Labelled<CodeVector>*> binaryCodes;
        Random::seed();

        // Create binary codes for positive samples
        int nrOfPositiveSamples = (int) positiveBoxList.size();

        int nrOfWarps = 3;
        int warpNo = 1;
        Frame* currentFrame = frame->clone();

        BoxHull* bbHull = new BoxHull();
        for (int i = 0; i < nrOfPositiveSamples; i++) {
            Box* box = positiveBoxList[i];
            bbHull->add(box);
        }
        Box* boxHull = bbHull->getBox();

        do {
            for (int i = 0; i < nrOfPositiveSamples; i++) {
                Box* box = positiveBoxList[i];
                if (box->variance > 0.0) {
                    CodeVector* codeVector = generateBinaryCode(currentFrame, box);
                    binaryCodes.push_back(new Labelled<CodeVector>(codeVector, 1));
                }
            }

            delete currentFrame;
            currentFrame = Frame::warp(frame, boxHull, angle, scale, shift);
            warpNo++;
        } while (warpNo <= nrOfWarps);

        delete currentFrame;
        delete bbHull;
        delete boxHull;

        // Create binary codes for negative samples
        int nrOfNegativeSamples = (int) negativeBoxList.size();
        for (int i = 0; i < nrOfNegativeSamples; i++) {
            Box* box = negativeBoxList[i];
            CodeVector* codeVector = generateBinaryCode(frame, box);
            binaryCodes.push_back(new Labelled<CodeVector>(codeVector, 0));
        }

        return binaryCodes;
}

void EnsembleClassifier::train(TrainingSet<ScoredBox> ts, int modelId, double varianceThreshold) {
    Frame* frame = ts.frame;
    vector<ScoredBox*> positiveBoxList = ts.positiveSamples;
    vector<ScoredBox*> negativeBoxList = ts.negativeSamples;

    vector<Labelled<CodeVector>*> samples = generateSamples(frame, positiveBoxList, negativeBoxList, varianceThreshold);
    doTrain(samples, modelId);
}

vector<Labelled<CodeVector>*> EnsembleClassifier::generateSamples(
    Frame* frame,
    vector<ScoredBox*> positiveBoxList,
    vector<ScoredBox*> negativeBoxList,
    double varianceThreshold) {
        double angle = 10.0;
        double scale = 0.02;
        double shift = 0.02;

        vector<Labelled<CodeVector>*> binaryCodes;
        Random::seed();

        int nrOfPositiveSamples = (int) positiveBoxList.size();

        int nrOfWarps = 3;
        int warpNo = 1;
        Frame* currentFrame = frame->clone();

        BoxHull* bbHull = new BoxHull();
        for (int i = 0; i < nrOfPositiveSamples; i++) {
            Box* box = positiveBoxList[i]->box;
            bbHull->add(box);
        }
        Box* boxHull = bbHull->getBox();

        do {
            for (int i = 0; i < nrOfPositiveSamples; i++) {
                Box* box = positiveBoxList[i]->box;
                CodeVector* codeVector = generateBinaryCode(currentFrame, box);
                binaryCodes.push_back(new Labelled<CodeVector>(codeVector, 1));
            }

            currentFrame = Frame::warp(frame, boxHull, angle, scale, shift);
            warpNo++;
        } while (warpNo <= nrOfWarps);

        free(bbHull);
        free(boxHull);

        // Create binary codes for negative samples
        int nrOfNegativeSamples = (int) negativeBoxList.size();
        for (int i = 0; i < nrOfNegativeSamples; i++) {
            EnsembleScore* score = (EnsembleScore*) negativeBoxList[i]->getScore("ensemble");
            CodeVector* codeVector = score->getCodeVector();
            binaryCodes.push_back(new Labelled<CodeVector>(codeVector, 0));
        }

        return binaryCodes;
}

void EnsembleClassifier::doTrain(vector<Labelled<CodeVector>*> samples, int modelId) {
    int nrOfBootstrap = 2;

    int nrOfSamples = (int) samples.size();
    int step = nrOfSamples / 10;

    for (int trial = 0; trial < nrOfBootstrap; trial++) {
        for (int i = 0; i < step; i++) {
            for (int k = 0; k < 10; k++) {
                int sampleIndex = k * step + i;

                Labelled<CodeVector>* labelledBinaryCode = samples[sampleIndex];
                CodeVector* codeVector = labelledBinaryCode->item;
                int label = labelledBinaryCode->label;

                double probability = getProbability(codeVector, modelId);

                if (label == 1) {
                    if (probability <= positiveUpdateThreshold) {
                        //printf("%4d. ENC(+) >>> update\n", sampleIndex);
                        updateBaseClassifiers(codeVector, modelId, true);
                    } else {
                        // printf("%4d. ENC(+) >>> ignore\n", sampleIndex);
                    }
                }

                if (label == 0) {
                    if (probability >= negativeUpdateThreshold) {
                        // printf("%4d. ENC(-) >>> update", sampleIndex);
                        updateBaseClassifiers(codeVector, modelId, false);
                    } else {
                        // printf("%4d. ENC(-) >>> ignore\n", sampleIndex);
                    }
                }

                // for (int i = 0; i < codeVector->size; i++) {
                //     printf("%d, ", codeVector->get(i));
                // }
                // printf("\n");
            }
        }
    }

    for (int i = 0; i < nrOfSamples; i++) {
        Labelled<CodeVector>* labelledBinaryCode = samples[i];
        delete labelledBinaryCode->item;
        delete labelledBinaryCode;
    }
}

void EnsembleClassifier::updateBaseClassifiers(CodeVector* codeVector, int modelId, bool label) {
    for (int i = 0; i < nrOfBaseClassifiers; i++) {
        BaseClassifier* bc = baseClassifiers[i];
        int binaryCode = codeVector->get(i);

        bc->train(binaryCode, modelId, label);
    }
}

//Classify Logic

bool EnsembleClassifier::classify(Frame* frame, ScoredBox* scoredBox) {
    vector<double> scores(nrOfModels);
    EnsembleScore* score = new EnsembleScore(nrOfModels, nrOfBaseClassifiers);
    for (int i = 0; i < nrOfBaseClassifiers; i++) {
        BaseClassifier* bc = baseClassifiers[i];
        vector<double> bcScores = bc->score(frame, scoredBox->box, score);

        for (int j = 0; j < nrOfModels; j++) {
            scores[j] += bcScores[j];
        }
    }

    bool anyModelClassified = false;
    vector<int> classifiedModelIds;
    for (int j = 0; j < nrOfModels; j++) {
        double score = scores[j];
        if (score > positiveLabelThreshold) {
            anyModelClassified = true;
            classifiedModelIds.push_back(j);
        }
    }

    score->scores = scores;
    score->isAnyModellClassified = anyModelClassified;
    score->classifiedModelIds = classifiedModelIds;
    scoredBox->withScore("ensemble", score);

    return score->isAnyModellClassified;
}

void EnsembleClassifier::dumpEnsembleClassifier() {
    for (int i = 0; i < nrOfBaseClassifiers; i++) {
        BaseClassifier* bc = baseClassifiers[i];
        bc->dumpBaseClassifier();
    }
}


//Remove this code
void EnsembleClassifier::score(Frame* frame, ScoredBox* scoredBox) {
    if (scoredBox->isScored("ensemble")) {
        // Already defined!
    } else {
        EnsembleScore* ensembleScore = new EnsembleScore(nrOfModels, nrOfBaseClassifiers);
        Box* box = scoredBox->box;

        for (int i = 0; i < nrOfBaseClassifiers; i++) {
            BaseClassifier* bc = baseClassifiers[i];
            bc->generateBinaryCode(frame, box, ensembleScore);
        }

        scoredBox->withScore("ensemble", ensembleScore);
    }
}
