#ifndef OBJECT_MODEL_H
#define OBJECT_MODEL_H

#include <vector>
#include "detector/nn/Patch.hpp"

using namespace std;

class ObjectModel {
private:
    vector<Patch*> positivePatchList;
    vector<Patch*> negativePatchList;
public:
    float computeRelativeScore(Patch* patch);
    float computeConservativeScore(Patch* patch);
};
#endif
