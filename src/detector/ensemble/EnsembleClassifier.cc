#include "detector/ensemble/EnsembleClassifier.hpp"

EnsembleClassifier::EnsembleClassifier() {
        baseClassifiers = generateBaseClassifier();
        nrOfBaseClassifiers = (int) baseClassifiers.size();
        minimumPositiveThreshold = 0.0;
        classifierName = "ensemble"
}

vector<BaseClassifier*> EnsembleClassifier::generateBaseClassifier() {
    vector<PixelComparison*> allComparisons = produceAllComparisons();
    vector<BaseClassifier*> classifiers = shuffleComparisons(allComparisons);
    return classifiers;
}

vector<PixelComparison*> EnsembleClassifier::produceAllComparisons() {
    vector<PixelComparison*> allComparisons;
    return classifiers;
}

vector<BaseClassifier*> EnsembleClassifier::shuffleComparisons(vector<PixelComparison*> allComparisons) {
    vector<BaseClassifier*> classifiers;
    return classifiers;
}

void EnsembleClassifier::init(Frame* frame, Box* box) {
    //TODO NotImplemented
}

bool EnsembleClassifier::classify(Frame* frame, ScoredBox* scoreBox) {
    double score = 0.0;
    Box* box = scoreBox->box;
    for (int i = 0; i < nrOfBaseClassifiers; i++) {
        BaseClassifier* bc = baseClassifiers[i];
        score += bc->score(frame, box);
    }
    scoreBox->withScore(classifierName, score);
    return score > minimumPositiveThreshold)
}

void EnsembleClassifier::update(Frame* frame, Box* box, DetectResult* detectResult) {
    //TODO NotImplemented
}
