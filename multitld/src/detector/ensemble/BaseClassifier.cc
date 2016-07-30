#include "detector/ensemble/BaseClassifier.hpp"

BaseClassifier::BaseClassifier(int id, vector<PixelComparison*> comparisons) {
    this->id = id;
    this->codeGen = new CodeGenerator(comparisons);
    this->decTree = new DecisionTree();
}

vector<float> BaseClassifier::score(Frame* frame, Box* box, EnsembleScore* score) {
    int binaryCode = codeGen->generateBinaryCode(frame, box);
    score->binaryCodes[id] = binaryCode;
    vector<float> probabilities = decTree->getProbabilities(binaryCode);
    return probabilities;
}

void BaseClassifier::train(Frame* frame, Box* box, int modelId, bool label) {
    int binaryCode = codeGen->generateBinaryCode(frame, box);
    decTree->update(binaryCode, modelId, label);
}

void BaseClassifier::train(Frame* frame, ScoredBox* scoredBox, int modelId, bool label) {
    EnsembleScore* ensembleScore = (EnsembleScore*) scoredBox->getScore("ensemble");
    int binaryCode = ensembleScore->binaryCodes[id];
    decTree->update(binaryCode, modelId, label);
}
