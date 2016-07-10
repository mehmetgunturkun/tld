#include "detector/ensemble/Branch.hpp"

vector<float> Branch::getProbabilities() {
    vector<float> probabilities(nrOfLeaves);
    for (int i = 0; i < nrOfLeaves; i++) {
        Leaf* leaf = branch[i];
        probabilities[i] = leaf->probability;
    }
    return probabilities;
}
