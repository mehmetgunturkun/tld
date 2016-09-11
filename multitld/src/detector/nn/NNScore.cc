#include "detector/nn/NNScore.hpp"

NNScore::NNScore(Patch* patch, vector<float> relativeScores, vector<float> conservativeScores) {
    this->patch = patch;
    this->relativeScores = relativeScores;
    this->conservativeScores = conservativeScores;
}

NNScore::NNScore() {
    this->patch = NULL;
}

Score* NNScore::merge(Score* other) {
    NNScore* otherNNScore = (NNScore*) other;
    NNScore* newNNScore = new NNScore();

    for (int i = 0; i < TLDConfig::nrOfModels; i++) {
        float thisRelativeScore = relativeScores[i];
        float otherRelativeScore = otherNNScore->relativeScores[i];
        newNNScore->relativeScores[i] = (thisRelativeScore + otherRelativeScore) / 2;

        float thisConervativeScore = conservativeScores[i];
        float otherConservativeScore = otherNNScore->conservativeScores[i];
        newNNScore->conservativeScores[i] = (thisConervativeScore + otherConservativeScore) / 2;
    }

    return newNNScore;
}
