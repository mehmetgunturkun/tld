#ifndef BRANCH_H
#define BRANCH_H

#include "common/Logging.hpp"
#include "detector/ensemble/Leaf.hpp"

class Branch {
private:
    vector<Leaf*> branch;
    int nrOfLeaves;

public:
    bool dirtyBit;

    Branch(int nrOfModels);
    ~Branch();

    double getProbability(int modelId);
    vector<double> getProbabilities();

    void update(int modelId, bool label);
    void dumpLeaves();
};

#endif
