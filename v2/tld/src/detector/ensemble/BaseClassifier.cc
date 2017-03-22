#include "detector/ensemble/BaseClassifier.hpp"

BaseClassifier::BaseClassifier(int id, vector<PixelComparison*> comparisons, int nrOfModels) {
    this->id = id;
    this->codeGen = new CodeGenerator(comparisons);
    this->decTree = new DecisionTree(nrOfModels);
}

int BaseClassifier::generateBinaryCode(Frame* frame, Box* box) {
    int binaryCode = codeGen->generateBinaryCode(frame, box);
    return binaryCode;
}

double BaseClassifier::getProbability(int binaryCode, int modelId) {
    return decTree->getProbability(binaryCode, modelId);
}

void BaseClassifier::train(int binaryCode, int modelId, bool label) {
    decTree->update(binaryCode, modelId, label);
}
