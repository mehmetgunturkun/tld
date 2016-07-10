#include "detector/ensemble/BaseClassifier.hpp"

BaseClassifier::BaseClassifier(int i, vector<PixelComparison*> comparisons) {
    this->codeGen = new CodeGenerator(comparisons);
    this->decTree = new DecisionTree();
}

vector<float> BaseClassifier::score(Frame* frame, ScoredBox* box) {
    int binaryCode = codeGen->generateBinaryCode(frame, box->box);
    vector<float> probabilities = decTree->getProbabilities(binaryCode);
    return probabilities;
}
