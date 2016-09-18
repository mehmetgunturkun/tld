#include "detector/nn/NNScore.hpp"

NNScore::NNScore(Patch* patch, vector<float> relativeScores, vector<float> conservativeScores) {
    this->patch = patch;
    this->relativeScores = relativeScores;
    this->conservativeScores = conservativeScores;
}

NNScore::NNScore() {
    this->patch = NULL;
}

NNScore::NNScore(Patch* patch) {
    this->patch = patch;
}

Score* NNScore::clone() {
    NNScore* clone = new NNScore();
    clone->patch = this->patch;
    clone->relativeScores =this->relativeScores;
    clone->conservativeScores = this->conservativeScores;
    return clone;
}

Score* NNScore::sum(Score* other) {
    NNScore* otherNNScore = (NNScore*) other;
    NNScore* newNNScore = (NNScore*) this->clone();

    for (int i = 0; i < TLDConfig::nrOfModels; i++) {
        float thisRelativeScore = relativeScores[i];
        float otherRelativeScore = otherNNScore->relativeScores[i];
        newNNScore->relativeScores[i] = thisRelativeScore + otherRelativeScore;

        float thisConervativeScore = conservativeScores[i];
        float otherConservativeScore = otherNNScore->conservativeScores[i];
        newNNScore->conservativeScores[i] = thisConervativeScore + otherConservativeScore;
    }

    return newNNScore;
}

Score* NNScore::divide(int n) {
    int m = relativeScores.size();
    for (int i = 0; i < m; i++) {
        relativeScores[i] = relativeScores[i] / n;
        conservativeScores[i] = conservativeScores[i] / n;
    }
    return this;
}
