#include "detector/ensemble/EnsembleClassifier.hpp"

EnsembleClassifier::EnsembleClassifier() {
        Random::seed();
        classifierName = Conf::getString("detector.ensemble.classifierName",  "ensemble");
        nrOfBaseClassifiers = Conf::getInt("detector.ensemble.nrOfBaseClassifiers",  10);
        nrOfPixelComparisons = Conf::getInt("detector.ensemble.nrOfPixelComparisons",  13);
        baseClassifiers = generateBaseClassifier();

        minimumPositiveThreshold = Conf::getDouble("detector.ensemble.positiveThreshold", 0.5);
}

vector<BaseClassifier*> EnsembleClassifier::generateBaseClassifier() {
    vector<PixelComparison*> allComparisons = produceAllComparisons();
    vector<BaseClassifier*> classifiers = shuffleComparisons(allComparisons);
    return classifiers;
}

Point2f* shift(Point2f* point, double x, double y) {
    Point2f* shiftedPoint = new Point2f(point->x + x, point->y + y);
    return shiftedPoint;
}

vector<Point2f*> shiftPoints(vector<Point2f*> points, int size, double x, double y) {
    vector<Point2f*> shiftedPoints;
    for (int i = 0; i < size; i++) {
        Point2f* point = points[i];
        Point2f* shiftedPoint = shift(point, x, y);
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
    double random = (double) Random::randomFloat();
    vector<Point2f*> rightShiftedList = shiftPoints(toList, nrOfPoints, random + 0.2, 0.0);

    // printf("------- L ------\n");
    random = (double) Random::randomFloat();
    vector<Point2f*> leftShiftedList = shiftPoints(toList, nrOfPoints, -(random + 0.2), 0.0);

    // printf("------- T ------\n");
    random = (double) Random::randomFloat();
    vector<Point2f*> topShiftedList = shiftPoints(toList, nrOfPoints, 0.0, -(random + 0.2));

    // printf("------- B ------\n");
    random = (double) Random::randomFloat();
    vector<Point2f*> bottomShiftedList = shiftPoints(toList, nrOfPoints, 0.0, random + 0.2);

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
    allComparisons = Random::randomSample(allComparisons, nrOfComparisons);

    vector<BaseClassifier*> classifiers;
    for (int i = 0; i < nrOfBaseClassifiers; i++) {
        vector<PixelComparison*> comparisons;
        for (int j = 0; j < nrOfPixelComparisons; j++) {
            PixelComparison* pc = allComparisons[i*nrOfPixelComparisons+j];
            comparisons.push_back(pc);
        }
        BaseClassifier* classifier = new BaseClassifier(i, comparisons);
        classifiers.push_back(classifier);
    }
    return classifiers;
}

void EnsembleClassifier::init(TrainingSet<Box> ts) {
    vector<Labelled<Box>> samples = ts.getLabelledSamples();
    for (int i = 0; i < ts.nrOfSamples; i++) {
        Labelled<Box> sample = samples[i];

        Frame* frame = sample.frame;
        Box* box = sample.item;
        int label = sample.label;

        EnsembleClassificationDetails* details = score(frame, box);

        println("Label:%d, Score: %g", label, details->score);

        if (label == 1 && details->score < 5) {
            for (int j = 0; j < nrOfBaseClassifiers; j++) {
                BaseClassifier* bc = baseClassifiers[j];
                bc->init(frame, box, true);
            }
        } else if (label == 0 && details->score > 5) {
            for (int j = 0; j < nrOfBaseClassifiers; j++) {
                BaseClassifier* bc = baseClassifiers[j];
                bc->init(frame, box, false);
            }
        }
    }
}

EnsembleClassificationDetails* EnsembleClassifier::score(Frame* frame, Box* box) {
    EnsembleClassificationDetails* detail = new EnsembleClassificationDetails(nrOfBaseClassifiers);
    for (int i = 0; i < nrOfBaseClassifiers; i++) {
        BaseClassifier* bc = baseClassifiers[i];
        int binaryCode = bc->generateBinaryCode(frame, box);
        double bcScore = bc->getProbability(binaryCode);
        detail->add(bc->id, binaryCode, bcScore);
    }
    return detail;
}

bool EnsembleClassifier::classify(Frame* frame, ScoredBox* scoreBox) {
    Box* box = scoreBox->box;
    EnsembleClassificationDetails* detail = this->score(frame, box);
    scoreBox->withScore(classifierName, detail);
    return detail->score > minimumPositiveThreshold;
}

void EnsembleClassifier::update(TrainingSet<ScoredBox> ts) {
    Frame* frame = ts.frame;
    vector<Labelled<ScoredBox>> samples = ts.getLabelledSamples();
    for (int i = 0; i < nrOfBaseClassifiers; i++) {
        BaseClassifier* bc = baseClassifiers[i];
        for (int i = 0; i < ts.nrOfSamples; i++) {
            Labelled<ScoredBox> sample = samples[i];
            ScoredBox* scoredBox = sample.item;
            if (sample.label == 1) {
                bc->update(frame, scoredBox, true);
            } else {
                bc->update(frame, scoredBox, false);
            }
        }
    }
}

void EnsembleClassifier::dumpBaseClassifiers() {
    for (int i = 0; i < nrOfBaseClassifiers; i++) {
        BaseClassifier* bc = baseClassifiers[i];
        bc->dumpDecisionTree();
    }
}
