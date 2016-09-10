#ifndef BRANCH_H
#define BRANCH_H

#include "detector/ensemble/Leaf.hpp"

class Branch {
private:
    vector<Leaf*> branch;
    int nrOfLeaves;

public:
    bool dirtyBit;
    Branch();
    vector<float> getProbabilities();
    float getProbability(int modelId);
    void update(int modelId, bool label);
    void dumpLeaves();
};

#endif
