#include "detector/ensemble/DecisionTree.hpp"

DecisionTree::DecisionTree(int nrOfModels) {
    tree.resize(pow(2, 13));
    for (int i = 0; i < pow(2, 13); i++) {
        tree[i] = new Branch(nrOfModels);
    }
}

vector<double> DecisionTree::getProbabilities(int binaryCode) {
    Branch* branch = tree[binaryCode];
    vector<double> probabilities = branch->getProbabilities();
    return probabilities;
}

double DecisionTree::getProbability(int binaryCode, int modelId) {
    Branch* branch = tree[binaryCode];
    double probability = branch->getProbability(modelId);
    return probability;
}

void DecisionTree::update(int binaryCode, int modelId, bool label) {
    Branch* branch = tree[binaryCode];
    branch->update(modelId, label);
}

void DecisionTree::dumpDecisionTree() {
    int nrOfBranch = pow(2, 13);
    for (int i = 0; i < nrOfBranch; i++) {
         Branch* branch = tree[i];
         if (branch->dirtyBit) {
             println("BinaryCode: %d", i);
             branch->dumpLeaves();
         }
    }
}
