#ifndef OBJECT_MODEL_H
#define OBJECT_MODEL_H

#include <vector>
#include "detector/nn/Patch.hpp"

using namespace std;

class ObjectModel {
public:
    int nrOfPositivePatches;
    vector<Patch*> positivePatchList;

    int nrOfNegativePatches;
    vector<Patch*> negativePatchList;

    ObjectModel();
    double computeRelativeScore(Patch* sample);
    double computeConservativeScore(Patch* sample);

    double computePositiveSimilarity(Patch* sample);
    double computeNegativeSimilarity(Patch* sample);
    double computeNearestSimilarity(vector<Patch*> templatePatchList, int size, Patch* sample);
    double computeSimilarity(Patch* templatePatch, Patch* samplePatch);

    void add(Patch* patch, bool label);
};
#endif
