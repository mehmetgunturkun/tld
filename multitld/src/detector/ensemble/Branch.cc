#include "detector/ensemble/Branch.hpp"

Branch::Branch(int nrOfModels) {
    nrOfLeaves = nrOfModels;
    dirtyBit = false;
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

float Branch::getProbability(int modelId) {
    Leaf* leaf = branch[modelId];
    float probability = leaf->probability;
    return probability;
}

void Branch::update(int modelId, bool label) {
    dirtyBit = true;
    Leaf* leaf = branch[modelId];
    if (label) {
        leaf->incrementPositive();
    } else {
        leaf->incrementNegative();
    }
}

void Branch::dumpLeaves() {
    for (int i = 0; i < nrOfLeaves; i++) {
        Leaf* leaf = branch[i];
        printf("\t%d: %s\n", i, leaf->toString().c_str());
    }
}
