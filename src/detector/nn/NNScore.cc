#include "detector/nn/NNScore.hpp"

NNScore::NNScore(Patch* patch, vector<double> relativeScores, vector<double> conservativeScores) {
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

NNScore::~NNScore() {
    delete this->patch;
}

Score* NNScore::clone() {
    NNScore* clone = new NNScore();
    clone->patch = this->patch->clone();
    clone->relativeScores =this->relativeScores;
    clone->conservativeScores = this->conservativeScores;
    return clone;
}

Score* NNScore::sum(Score* other) {
    NNScore* otherNNScore = (NNScore*) other;
    int nrOfModels = (int) this->relativeScores.size();

    for (int i = 0; i < nrOfModels; i++) {
        double thisRelativeScore = relativeScores[i];
        double otherRelativeScore = otherNNScore->relativeScores[i];
        this->relativeScores[i] = thisRelativeScore + otherRelativeScore;

        double thisConervativeScore = conservativeScores[i];
        double otherConservativeScore = otherNNScore->conservativeScores[i];
        this->conservativeScores[i] = thisConervativeScore + otherConservativeScore;
    }

    return this;
}

Score* NNScore::divide(int n) {
    int m = relativeScores.size();
    for (int i = 0; i < m; i++) {
        relativeScores[i] = relativeScores[i] / n;
        conservativeScores[i] = conservativeScores[i] / n;
    }
    return this;
}

double NNScore::getValue(int modelId) {
    return conservativeScores[modelId];
}
