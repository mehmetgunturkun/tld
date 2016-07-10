#ifndef DECISION_TREE_H
#define DECISION_TREE_H

#include "detector/ensemble/Branch.hpp"

class DecisionTree {
private:
    vector<Branch*> tree;
public:
    vector<float> getProbabilities(int binaryCode);
};

#endif
