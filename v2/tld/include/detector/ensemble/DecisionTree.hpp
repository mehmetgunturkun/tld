#ifndef DECISION_TREE_H
#define DECISION_TREE_H

#include <cmath>
#include "common/Logging.hpp"
#include "detector/ensemble/Branch.hpp"

class DecisionTree {
private:
    vector<Branch*> tree;
public:
    DecisionTree(int nrOfModels);

    double getProbability(int binaryCode, int modelId);
    vector<double> getProbabilities(int binaryCode);

    void update(int binaryCode, int modelId, bool label);
    void dumpDecisionTree();
};

#endif
