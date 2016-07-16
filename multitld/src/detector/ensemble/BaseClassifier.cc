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
