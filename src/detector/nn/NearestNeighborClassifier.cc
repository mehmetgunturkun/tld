#include "detector/nn/NearestNeighborClassifier.hpp"

NearestNeighborClassifier::NearestNeighborClassifier() {
    classifierName = Conf::getString("detector.nn.classifierName",  "nn");
    minimumPositiveThreshold = Conf::getDouble("detector.nn.positiveThreshold",  0.6);

    model = new ObjectModel();
}

void NearestNeighborClassifier::init(TrainingSet<Box> ts) {
    vector<Labelled<Box>> samples = ts.getLabelledSamples();
    for (int i = 0; i < ts.nrOfSamples; i++) {
        println("mc1000");
        Labelled<Box> sample = samples[i];
        Frame* frame = sample.frame;
        Box* box = sample.item;
        int label = sample.label;
        println("mc2000 - %d", box->id);
        Patch* patch = new Patch(frame, box);
        println("mc3000");
        model->add(patch, label == 1);
    }
}

bool NearestNeighborClassifier::classify(Frame* frame, ScoredBox* scoredBox) {
    Box* box = scoredBox->box;
    Patch* sample = new Patch(frame, box);
    double score = model->computeRelativeScore(sample);
    NNClassificationDetails* detail = new NNClassificationDetails(sample, score);
    scoredBox->withScore(classifierName, detail);
    return score > minimumPositiveThreshold;
}

void NearestNeighborClassifier::update(TrainingSet<ScoredBox> ts) {
    vector<ScoredBox*> positiveSamples = ts.positiveSamples;
    for (int i = 0; i < ts.nrOfPositiveSamples; i++) {
        ScoredBox* scoredBox = positiveSamples[i];
        NNClassificationDetails* details = (NNClassificationDetails*) scoredBox->getDetail("nn");
        Patch* patch = details->patch;
        double score = model->computeRelativeScore(patch);
        if (score < 0.65) {
            model->add(patch, true);
        }
    }

    vector<ScoredBox*> negativeSamples = ts.negativeSamples;
    for (int i = 0; i < ts.nrOfNegativeSamples; i++) {
        ScoredBox* scoredBox = negativeSamples[i];
        NNClassificationDetails* details = (NNClassificationDetails*) scoredBox->getDetail("nn");
        Patch* patch = details->patch;
        double score = model->computeRelativeScore(patch);
        if (score > 0.5) {
            model->add(patch, false);
        }
    }
}

double NearestNeighborClassifier::score(Frame* frame, Box* box) {
    Patch* sample = new Patch(frame, box);
    double score = model->computeConservativeScore(sample);
    return score;
}

void NearestNeighborClassifier::dumpObjectModel() {
    println("There are %d positive and %d negative patches in object model!",
        model->nrOfPositivePatches,
        model->nrOfNegativePatches
    );
}
