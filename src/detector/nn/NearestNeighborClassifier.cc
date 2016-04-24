#include "detector/nn/NearestNeighborClassifier.hpp"

NearestNeighborClassifier::NearestNeighborClassifier() {
    classifierName = Conf::getString("detector.nn.classifierName",  "nn");
    minimumPositiveThreshold = Conf::getDouble("detector.nn.positiveThreshold",  0.6);
    model = new ObjectModel();
}

void NearestNeighborClassifier::init(TrainingSet<Box> ts) {
    vector<Labelled<Box>> samples = ts.getLabelledSamples();
    for (int i = 0; i < ts.nrOfSamples; i++) {
        Labelled<Box> sample = samples[i];
        Frame* frame = sample.frame;
        Box* box = sample.item;
        int label = sample.label;
        Patch* patch = new Patch(frame, box);
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
    println("mc17000");
    vector<Labelled<ScoredBox>> samples = ts.getLabelledSamples();
    println("mc18000");
    for (int i = 0; i < ts.nrOfSamples; i++) {
        println("mc19000");
        Labelled<ScoredBox> sample = samples[i];
        ScoredBox* box = sample.item;

        int label = sample.label;

        NNClassificationDetails* details = (NNClassificationDetails*) box->getDetail(classifierName);
        println("mc20000");
        Patch* patch = details->patch;
        println("mc21000");
        model->add(patch, label == 1);
        println("mc22000");
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
