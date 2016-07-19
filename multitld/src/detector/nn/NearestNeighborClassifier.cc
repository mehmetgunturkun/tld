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
    return score->isAnyModelDetected;
}

void NearestNeighborClassifier::train(TrainingSet<Box> ts, int modelId) {
    
}

void NearestNeighborClassifier::train(TrainingSet<ScoredBox> ts, int modelId) {

}
