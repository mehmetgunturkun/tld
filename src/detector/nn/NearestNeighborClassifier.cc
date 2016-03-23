#include "detector/nn/NearestNeighborClassifier.hpp"

NearestNeighborClassifier::NearestNeighborClassifier() {
    model = new ObjectModel();
    minimumPositiveThreshold = 0.6;
}

void NearestNeighborClassifier::init(TrainingSet<Box> ts) {
    Frame* frame = ts.frame;

    vector<Box*> positiveSamples = ts.positiveSamples;
    for (int i = 0; i < ts.nrOfPositiveSamples; i++) {
        Box* box = positiveSamples[i];
        Patch* patch = new Patch(frame, box);
        model->add(patch, true);
    }

    vector<Box*> negativeSamples = ts.negativeSamples;
    for (int i = 0; i < ts.nrOfNegativeSamples; i++) {
        Box* box = negativeSamples[i];
        Patch* patch = new Patch(frame, box);
        model->add(patch, false);
    }
}

bool NearestNeighborClassifier::classify(Frame* frame, ScoredBox* scoredBox) {
    Box* box = scoredBox->box;
    Patch* sample = new Patch(frame, box);
    double score = model->computeRelativeScore(sample);
    scoredBox->withScore("UNDEFINED", score);
    return score > minimumPositiveThreshold;
}

void NearestNeighborClassifier::update(TrainingSet<ScoredBox> ts) {

}

double NearestNeighborClassifier::score(Frame* frame, Box* box) {
    Patch* sample = new Patch(frame, box);
    double score = model->computeConservativeScore(sample);
    return score;
}
