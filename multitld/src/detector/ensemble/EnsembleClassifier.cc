#include "detector/ensemble/EnsembleClassifier.hpp"
#include "util/ImageBuilder.hpp"

#define POS_X_DIR 1
#define NEG_X_DIR 2
#define POS_Y_DIR 3
#define NEG_Y_DIR 4

EnsembleClassifier::EnsembleClassifier(Frame* firstFrame, vector<Box*> boxList) {
    Random::seed();
    this->classifierName = "ensemble";

    this->nrOfModels = (int) boxList.size();
    this->nrOfBaseClassifiers = 10;
    this->nrOfPixelComparisons = 13;

    this->positiveUpdateThreshold = 0.5 * nrOfBaseClassifiers;
    this->negativeUpdateThreshold = 0.5 * nrOfBaseClassifiers;
    this->POSITIVE_SCORE_THRESHOLD = 0.5 * nrOfBaseClassifiers;

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

// vector<Point2f*> shiftPoints(vector<Point2f*> points, int size, double x, double y) {
//     vector<Point2f*> shiftedPoints;
//     for (int i = 0; i < size; i++) {
//         Point2f* point = points[i];
//         Point2f* shiftedPoint = shift(point, x, y);
//         shiftedPoints.push_back(shiftedPoint);
//     }
//     return shiftedPoints;
// }

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
    int nrOfComparisons = (int) allComparisons.size();
    for (int i = 0; i < nrOfComparisons; i++) {
        printf("%s\n", allComparisons[i]->toString().c_str());
    }

    printf("===============================\n");

    allComparisons = Random::randomSample(allComparisons);
    for (int i = 0; i < nrOfComparisons; i++) {
        printf("%s\n", allComparisons[i]->toString().c_str());
    }

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
    vector<float> scores(nrOfModels);
    EnsembleScore* score = new EnsembleScore(nrOfModels, nrOfBaseClassifiers);
    for (int i = 0; i < nrOfBaseClassifiers; i++) {
        BaseClassifier* bc = baseClassifiers[i];
        vector<float> bcScores = bc->score(frame, scoredBox->box, score);

        for (int j = 0; j < nrOfModels; j++) {
            scores[j] += bcScores[j];
        }
    }

    bool anyModelClassified = false;
    vector<int> classifiedModelIds;
    for (int j = 0; j < nrOfModels; j++) {
        float score = scores[j];
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
            int binaryCode = bc->generateBinaryCode(frame, box, ensembleScore);
            printf("BC(%d).binaryCode(%s) >> %d\n", i, box->toCharArr(), binaryCode);
        }

        scoredBox->withScore("ensemble", ensembleScore);
    }
}

void EnsembleClassifier::train(TrainingSet<ScoredBox> ts, int modelId) {
    vector<Labelled<ScoredBox>*> samples = ts.getLabelledSamples();
    int nrOfBootstrap = ts.nrOfBootstrap;
    // printf("EC >> %lu samples are going to be processed for training - online\n", samples.size());
    for (int trial = 0; trial < nrOfBootstrap; trial++) {
        for (int i = 0; i < ts.nrOfSamples; i++) {
            Labelled<ScoredBox>* sample = samples[i];

            Frame* frame = sample->frame;
            ScoredBox* scoredBox = sample->item;
            int label = sample->label;

            float probability = getProbability(scoredBox, modelId);

            // ImageBuilder* builder = new ImageBuilder(frame);
            // printf("EC >> Prob(%d): %f\n", label, probability);

            if (label == true && probability < positiveUpdateThreshold) {
                // builder->withBox(scoredBox->box, Colors::BLUE)->withTitle("ensemble-pos")->display(1000);
                updateBaseClassifiers(frame, scoredBox, modelId, true);
            }

            if (label == false && probability > negativeUpdateThreshold) {
                // builder->withBox(scoredBox->box, Colors::RED)->withTitle("ensemble-neg")->display(1000);
                updateBaseClassifiers(frame, scoredBox, modelId, false);
            }
        }
    }
    // printf("EC >> %lu samples were processed for training\n", samples.size());
}

float EnsembleClassifier::getProbability(ScoredBox* scoredBox, int modelId) {
    float probability = 0.0f;
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
