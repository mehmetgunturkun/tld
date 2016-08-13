#include "detector/nn/NNScore.hpp"

NNScore::NNScore(Patch* patch, vector<float> relativeScores, vector<float> conservativeScores) {
    this->patch = patch;
    this->relativeScores = relativeScores;
    this->conservativeScores = conservativeScores;
}
