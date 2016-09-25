#ifndef DECISION_TREE_H
#define DECISION_TREE_H

#include <cmath>
#include "detector/ensemble/Branch.hpp"

class DecisionTree {
private:
    vector<Branch*> tree;
public:
    DecisionTree(int nrOfModels);
    vector<float> getProbabilities(int binaryCode);
    float getProbability(int binaryCode, int modelId);
    void update(int binaryCode, int modelId, bool label);
    void dumpDecisionTree();
};

#endif
