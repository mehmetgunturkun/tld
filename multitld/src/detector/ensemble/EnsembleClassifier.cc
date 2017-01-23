#include "detector/ensemble/EnsembleClassifier.hpp"
#include "util/ImageBuilder.hpp"

#define POS_X_DIR 1
#define NEG_X_DIR 2
#define POS_Y_DIR 3
#define NEG_Y_DIR 4

EnsembleClassifier::EnsembleClassifier() {
    this->classifierName = "ensemble";

    this->nrOfBaseClassifiers = 10;
    this->nrOfPixelComparisons = 13;

    this->positiveUpdateThreshold = 0.5 * nrOfBaseClassifiers;
    this->negativeUpdateThreshold = 0.5 * nrOfBaseClassifiers;
    this->POSITIVE_SCORE_THRESHOLD = 0.5 * nrOfBaseClassifiers;
}

void EnsembleClassifier::init(Frame* firstFrame, vector<Box*> boxList) {
    this->nrOfModels = (int) boxList.size();
    this->baseClassifiers = generateBaseClassifier();
}

vector<BaseClassifier*> EnsembleClassifier::generateBaseClassifier() {
    vector<PixelComparison*> allComparisons = produceAllComparisons();
    printf("Number of all comparisons: %d\n", (int) allComparisons.size());

    vector<BaseClassifier*> classifiers = shuffleComparisons(allComparisons);
    printf("Ensemble Classifier is created with %lu base classifiers\n", classifiers.size());
    return classifiers;
}

Point2f* shift(Point2f* point, double x, double y) {
    Point2f* shiftedPoint = new Point2f(point->x + x, point->y + y);
    return shiftedPoint;
}

Point2f* shift(Point2f* point, int direction) {
    Point2f* shiftedPoint;
    double random = (double) Random::randomFloat();
    switch (direction)
    {
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

vector<Point2f*> shiftPoints(vector<Point2f*> points, int size, int direction) {
    vector<Point2f*> shiftedPoints;
    for (int i = 0; i < size; i++) {
        Point2f* point = points[i];
        Point2f* shiftedPoint = shift(point, direction);
        shiftedPoints.push_back(shiftedPoint);
    }
    return shiftedPoints;
}

vector<PixelComparison*> EnsembleClassifier::produceAllComparisons() {
    vector<Point2f*> fromList;
    for (float i = 0.0; i <= 1.0; i = i + 0.2) {
        for (float j = 0.0; j <= 1.0; j = j + 0.2) {
            fromList.push_back(new Point2f(i, j));
        }
    }

    int nrOfPoints = (int) fromList.size();
    for (int i = 0; i < nrOfPoints; i++) {
        Point2f* fromPoint = fromList[i];
        Point2f* newFromPoint = new Point2f(fromPoint->x + 0.1, fromPoint->y + 0.1);
        fromList.push_back(newFromPoint);
    }

    nrOfPoints *= 2;
    vector<Point2f*> toList;
    toList.insert(toList.end(), fromList.begin(), fromList.end());

    // printf("------- R ------\n");
    // double random = (double) Random::randomFloat();
    vector<Point2f*> rightShiftedList = shiftPoints(toList, nrOfPoints, POS_X_DIR);

    // printf("------- L ------\n");
    // random = (double) Random::randomFloat();
    vector<Point2f*> leftShiftedList = shiftPoints(toList, nrOfPoints, NEG_X_DIR);

    // printf("------- T ------\n");
    // random = (double) Random::randomFloat();
    vector<Point2f*> topShiftedList = shiftPoints(toList, nrOfPoints, NEG_Y_DIR);

    // printf("------- B ------\n");
    // random = (double) Random::randomFloat();
    vector<Point2f*> bottomShiftedList = shiftPoints(toList, nrOfPoints, POS_Y_DIR);

    vector<Point2f*> finalFromList;
    vector<Point2f*> finalToList;

    for (int i = 0; i < nrOfPoints; i++) {
        Point2f* from = fromList[i];
        Point2f* rightShiftedPoint = rightShiftedList[i];
        if (from->x > 0 && from->x < 1 && from->y > 0 && from->y < 1) {
            finalFromList.push_back(from);
            finalToList.push_back(rightShiftedPoint);
        }
    }

    for (int i = 0; i < nrOfPoints; i++) {
        Point2f* from = fromList[i];
        Point2f* leftShiftedPoint = leftShiftedList[i];
        if (from->x > 0 && from->x < 1 && from->y > 0 && from->y < 1) {
            finalFromList.push_back(from);
            finalToList.push_back(leftShiftedPoint);
        }
    }

    for (int i = 0; i < nrOfPoints; i++) {
        Point2f* from = fromList[i];
        Point2f* topShiftedPoint = topShiftedList[i];
        if (from->x > 0 && from->x < 1 && from->y > 0 && from->y < 1) {
            finalFromList.push_back(from);
            finalToList.push_back(topShiftedPoint);
        }
    }

    for (int i = 0; i < nrOfPoints; i++) {
        Point2f* from = fromList[i];
        Point2f* bottomShiftedPoint = bottomShiftedList[i];
        if (from->x > 0 && from->x < 1 && from->y > 0 && from->y < 1) {
            finalFromList.push_back(from);
            finalToList.push_back(bottomShiftedPoint);
        }
    }

    nrOfPoints = (int) finalFromList.size();
    vector<PixelComparison*> allComparisons(nrOfPoints);
    for (int i = 0; i < nrOfPoints; i++) {
        Point2f* fromPoint = finalFromList[i];
        Point2f* toPoint = finalToList[i];

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
    for (int i = 0; i < nrOfBaseClassifiers; i++) {
        vector<PixelComparison*> comparisons;
        for (int j = 0; j < nrOfPixelComparisons; j++) {
            PixelComparison* pc = allComparisons[i*nrOfPixelComparisons+j];
            comparisons.push_back(pc);
        }
        BaseClassifier* classifier = new BaseClassifier(i, comparisons, this->nrOfModels);
        classifiers.push_back(classifier);
    }
    return classifiers;
}

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
        if (score > POSITIVE_SCORE_THRESHOLD) {
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

double EnsembleClassifier::getProbability(ScoredBox* scoredBox, int modelId) {
    double probability = 0.0;
    for (int i = 0; i < nrOfBaseClassifiers; i++) {
        BaseClassifier* bc = baseClassifiers[i];
        probability += bc->getProbability(scoredBox, modelId);
    }
    return probability;
}

void EnsembleClassifier::updateBaseClassifiers(Frame* frame, ScoredBox* scoredBox, int modelId, bool label) {
    for (int i = 0; i < nrOfBaseClassifiers; i++) {
        BaseClassifier* bc = baseClassifiers[i];
        bc->train(frame, scoredBox, modelId, label);
    }
}

void EnsembleClassifier::dumpEnsembleClassifier() {
    for (int i = 0; i < nrOfBaseClassifiers; i++) {
        BaseClassifier* bc = baseClassifiers[i];
        bc->dumpBaseClassifier();
    }
}

//v2

CodeVector* EnsembleClassifier::generateBinaryCode(Frame* frame, Box* box) {
    CodeVector* codeVector = new CodeVector(nrOfBaseClassifiers);
    for (int i = 0; i < nrOfBaseClassifiers; i++) {
        BaseClassifier* bc = baseClassifiers[i];
        int binaryCode = bc->generateBinaryCode(frame, box);
        codeVector->set(i, binaryCode);
    }
    return codeVector;
}

vector<Labelled<CodeVector>*> EnsembleClassifier::generateSamples(
    Frame* frame,
    vector<Box*> positiveBoxList,
    vector<Box*> negativeBoxList) {
        vector<Labelled<CodeVector>*> binaryCodes;

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
                CodeVector* codeVector = generateBinaryCode(currentFrame, box);
                binaryCodes.push_back(new Labelled<CodeVector>(codeVector, 1));
            }

            currentFrame = Frame::warp(frame, boxHull);
            warpNo++;
        } while (warpNo <= nrOfWarps);

        free(bbHull);
        free(boxHull);

        // Create binary codes for negative samples
        int nrOfNegativeSamples = (int) negativeBoxList.size();
        for (int i = 0; i < nrOfNegativeSamples; i++) {
            Box* box = negativeBoxList[i];
            CodeVector* codeVector = generateBinaryCode(frame, box);
            binaryCodes.push_back(new Labelled<CodeVector>(codeVector, 0));
        }

        return binaryCodes;
}

vector<Labelled<CodeVector>*> EnsembleClassifier::generateSamples(
    Frame* frame,
    vector<ScoredBox*> positiveBoxList,
    vector<ScoredBox*> negativeBoxList) {
        vector<Labelled<CodeVector>*> binaryCodes;

        int nrOfPositiveSamples = (int) positiveBoxList.size();

        BoxHull* bbHull = new BoxHull();
        for (int i = 0; i < nrOfPositiveSamples; i++) {
            Box* box = positiveBoxList[i]->box;
            bbHull->add(box);
        }
        Box* boxHull = bbHull->getBox();

        int nrOfWarps = 1;
        int warpNo = 1;
        Frame* currentFrame = frame->clone();
        do {
            for (int i = 0; i < nrOfPositiveSamples; i++) {
                Box* box = positiveBoxList[i]->box;
                CodeVector* codeVector = generateBinaryCode(currentFrame, box);
                binaryCodes.push_back(new Labelled<CodeVector>(codeVector, 1));
            }

            currentFrame = Frame::warp(frame, boxHull);
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

double EnsembleClassifier::getProbability(CodeVector* codeVector, int modelId) {
    double probability = 0.0;
    for (int i = 0; i < nrOfBaseClassifiers; i++) {
        BaseClassifier* bc = baseClassifiers[i];
        int binaryCode = codeVector->get(i);

        probability += bc->getProbability(binaryCode, modelId);
    }
    return probability;
}

void EnsembleClassifier::updateBaseClassifiers(CodeVector* codeVector, int modelId, bool label) {
    for (int i = 0; i < nrOfBaseClassifiers; i++) {
        BaseClassifier* bc = baseClassifiers[i];
        int binaryCode = codeVector->get(i);

        bc->train(binaryCode, modelId, label);
    }
}

void EnsembleClassifier::doTrain(vector<Labelled<CodeVector>*> samples, int modelId) {
    int nrOfBootstrap = 2;

    int nrOfSamples = (int) samples.size();
    int step = nrOfSamples / 10;

    for (int trial = 0; trial < nrOfBootstrap; trial++) {
        for (int i = 0; i < step; i++) {
            for (int k = 0; k < 10; k++) {
                Labelled<CodeVector>* labelledBinaryCode = samples[k * step + i];
                CodeVector* codeVector = labelledBinaryCode->item;
                int label = labelledBinaryCode->label;

                double probability = getProbability(codeVector, modelId);

                if (label == 1 && probability <= positiveUpdateThreshold) {
                    updateBaseClassifiers(codeVector, modelId, true);
                }

                if (label == 0 && probability >= negativeUpdateThreshold) {
                    updateBaseClassifiers(codeVector, modelId, false);
                }
            }
        }
    }
}

void EnsembleClassifier::train(TrainingSet<Box> ts, int modelId) {
    Frame* frame = ts.frame;
    vector<Box*> positiveBoxList = ts.positiveSamples;
    vector<Box*> negativeBoxList = ts.negativeSamples;

    vector<Labelled<CodeVector>*> samples = generateSamples(frame, positiveBoxList, negativeBoxList);
    doTrain(samples, modelId);
}

void EnsembleClassifier::train(TrainingSet<ScoredBox> ts, int modelId) {
    Frame* frame = ts.frame;
    vector<ScoredBox*> positiveBoxList = ts.positiveSamples;
    vector<ScoredBox*> negativeBoxList = ts.negativeSamples;

    vector<Labelled<CodeVector>*> samples = generateSamples(frame, positiveBoxList, negativeBoxList);
    doTrain(samples, modelId);
}
