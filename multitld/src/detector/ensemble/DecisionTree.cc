#include "detector/ensemble/DecisionTree.hpp"

DecisionTree::DecisionTree() {
    tree.resize(pow(2, 13));
    for (int i = 0; i < pow(2, 13); i++) {
        tree[i] = new Branch();
    }
}

vector<float> DecisionTree::getProbabilities(int binaryCode) {
    Branch* branch = tree[binaryCode];
    vector<float> probabilities = branch->getProbabilities();
    return probabilities;
}

void DecisionTree::update(int binaryCode, int modelId, bool label) {
    Branch* branch = tree[binaryCode];
    branch->update(modelId, label);
}
