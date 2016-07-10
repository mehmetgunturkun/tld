#ifndef BRANCH_H
#define BRANCH_H

#include "detector/ensemble/Leaf.hpp"

class Branch {
private:
    vector<Leaf*> branch;
    int nrOfLeaves;
public:
    vector<float> getProbabilities();
};

#endif
