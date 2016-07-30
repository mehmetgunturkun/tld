#ifndef DECISION_TREE_H
#define DECISION_TREE_H

#include <cmath>
#include "detector/ensemble/Branch.hpp"

class DecisionTree {
private:
    vector<Branch*> tree;
public:
    DecisionTree();
    vector<float> getProbabilities(int binaryCode);
    void update(int binaryCode, int modelId, bool label);
};

#endif
