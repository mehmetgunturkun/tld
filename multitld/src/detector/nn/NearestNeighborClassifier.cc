#include "detector/nn/NearestNeighborClassifier.hpp"

NearestNeighborClassifier::NearestNeighborClassifier() {}

bool NearestNeighborClassifier::classify(Frame* frame, ScoredBox* scoredBox) {
    Box* box = scoredBox->box;
    Patch* patch = new Patch(frame, box);
    vector<float> relativeScores;
    vector<float> conservativeScores;
    for (int i = 0;  i < nrOfModels; i++) {
        ObjectModel* objectModel = models[i];
        float relativeScore = objectModel->computeRelativeScore(patch);
        float conservativeScore = objectModel->computeConservativeScore(patch);

        relativeScores[i] = relativeScore;
        conservativeScores[i] = conservativeScore;
    }

    NNScore* score = new NNScore(patch, relativeScores, conservativeScores);
    scoredBox->withScore("nn", score);
    return score->isAnyModelClassified;
}

void NearestNeighborClassifier::train(TrainingSet<Box> ts, int modelId) {
    vector<Labelled<Box>> samples = ts.getLabelledSamples();
    ObjectModel* model = models[modelId];
    for (int i = 0; i < ts.nrOfSamples; i++) {
        Labelled<Box> sample = samples[i];
        Frame* frame = sample.frame;
        Box* box = sample.item;
        int label = sample.label;
        Patch* patch = new Patch(frame, box);
        model->add(patch, label == 1);
    }
}

void NearestNeighborClassifier::train(TrainingSet<ScoredBox> ts, int modelId) {
    vector<Labelled<ScoredBox>> samples = ts.getLabelledSamples();
    ObjectModel* model = models[modelId];
    for (int i = 0; i < ts.nrOfSamples; i++) {
        Labelled<ScoredBox> sample = samples[i];
        ScoredBox* scoredBox = sample.item;
        int label = sample.label;

        NNScore* nnScore = (NNScore*) scoredBox->getScore("nn");
        Patch* patch = nnScore->patch;
        model->add(patch, label == 1);
    }
}
