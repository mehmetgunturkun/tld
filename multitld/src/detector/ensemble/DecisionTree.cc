#include "detector/ensemble/DecisionTree.hpp"

vector<float> DecisionTree::getProbabilities(int binaryCode) {
    Branch* branch = tree[binaryCode];
    vector<float> probabilities = branch->getProbabilities();
    return probabilities;
}
