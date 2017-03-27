#include "detector/ensemble/BaseClassifier.hpp"

BaseClassifier::BaseClassifier(int id, vector<PixelComparison*> comparisons, int nrOfModels) {
    this->id = id;
    this->codeGen = new CodeGenerator(comparisons);
    this->decTree = new DecisionTree(nrOfModels);
}

BaseClassifier::~BaseClassifier() {
    delete codeGen;
    delete decTree;
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

vector<double> BaseClassifier::score(Frame* frame, Box* box, EnsembleScore* score) {
    int binaryCode = codeGen->generateBinaryCode(frame, box);
    score->setBinaryCode(id, binaryCode);
    vector<double> probabilities = decTree->getProbabilities(binaryCode);
    return probabilities;
}

double BaseClassifier::score(Frame* frame, Box* box, EnsembleScore* score, int modelId) {
    int binaryCode = codeGen->generateBinaryCode(frame, box);
    score->setBinaryCode(id, binaryCode);
    double probability = decTree->getProbability(binaryCode, modelId);
    return probability;
}

int BaseClassifier::generateBinaryCode(Frame* frame, Box* box, EnsembleScore* score) {
    int binaryCode = this->generateBinaryCode(frame, box);
    score->setBinaryCode(id, binaryCode);
    return binaryCode;
}
