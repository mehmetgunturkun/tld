#include "detector/ensemble/Branch.hpp"

Branch::Branch() {
    nrOfLeaves = 1;
    branch.resize(nrOfLeaves);
    for (int i = 0; i < nrOfLeaves; i++) {
        branch[i] = new Leaf();
    }
}

vector<float> Branch::getProbabilities() {
    vector<float> probabilities(nrOfLeaves);
    for (int i = 0; i < nrOfLeaves; i++) {
        Leaf* leaf = branch[i];
        probabilities[i] = leaf->probability;
    }
    return probabilities;
}

void Branch::update(int modelId, bool label) {
    Leaf* leaf = branch[modelId];
    if (label) {
        leaf->incrementPositive();
    } else {
        leaf->incrementNegative();
    }
}