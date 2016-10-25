#include "detector/ensemble/BaseClassifier.hpp"

BaseClassifier::BaseClassifier(int id, vector<PixelComparison*> comparisons, int nrOfModels) {
    this->id = id;
    this->codeGen = new CodeGenerator(comparisons);
    this->decTree = new DecisionTree(nrOfModels);
}

int BaseClassifier::generateBinaryCode(Frame* frame, Box* box, EnsembleScore* score) {
    int binaryCode = codeGen->generateBinaryCode(frame, box);
    score->binaryCodes[id] = binaryCode;
    return binaryCode;
}

vector<double> BaseClassifier::score(Frame* frame, Box* box, EnsembleScore* score) {
    int binaryCode = codeGen->generateBinaryCode(frame, box);
    score->binaryCodes[id] = binaryCode;
    vector<double> probabilities = decTree->getProbabilities(binaryCode);
    return probabilities;
}

double BaseClassifier::score(Frame* frame, Box* box, EnsembleScore* score, int modelId) {
    int binaryCode = codeGen->generateBinaryCode(frame, box);
    score->binaryCodes[id] = binaryCode;
    double probability = decTree->getProbability(binaryCode, modelId);
    return probability;
}

void BaseClassifier::train(Frame* frame, Box* box, int modelId, bool label) {
    int binaryCode = codeGen->generateBinaryCode(frame, box);
    decTree->update(binaryCode, modelId, label);
}

double BaseClassifier::getProbability(ScoredBox* scoredBox, int modelId) {
    EnsembleScore* ensembleScore = (EnsembleScore*) scoredBox->getScore("ensemble");
    if (ensembleScore->isMerged) {
        printf("Yes it is merged!\n");
    }
    int binaryCode = ensembleScore->binaryCodes[id];
    return decTree->getProbability(binaryCode, modelId);
}

void BaseClassifier::train(Frame* frame, ScoredBox* scoredBox, int modelId, bool label) {
    EnsembleScore* ensembleScore = (EnsembleScore*) scoredBox->getScore("ensemble");
    int binaryCode = ensembleScore->binaryCodes[id];
    decTree->update(binaryCode, modelId, label);
}

void BaseClassifier::dumpBaseClassifier() {
    printf("===== BaseClassifier(%2d) =====\n", this->id);
    decTree->dumpDecisionTree();
    printf("===============================\n");
}

void BaseClassifier::printPixelComparison() {
    printf("===== BaseClassifier(%2d) =====\n", this->id);
    codeGen->printPixelComparison();
    printf("===============================\n");
}
