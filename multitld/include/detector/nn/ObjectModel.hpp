#ifndef OBJECT_MODEL_H
#define OBJECT_MODEL_H

#include <vector>
#include "detector/nn/Patch.hpp"

using namespace std;

class ObjectScore {
public:
    double relativeScore;
    double conservativeScore;
    bool isInNegative;
    bool isInPositive;

    ObjectScore(double relativeScore, double conservativeScore, bool isInPositive, bool isInNegative);
};

class ObjectModel {
public:
    int nrOfPositivePatches;
    vector<Patch*> positivePatchList;

    int nrOfNegativePatches;
    vector<Patch*> negativePatchList;

    ObjectModel();

    double computeInitialPositiveSimilarity(Patch* sample);
    double computePositiveSimilarity(Patch* sample);
    double computeNegativeSimilarity(Patch* sample);
    double computeNearestSimilarity(vector<Patch*> templatePatchList, int size, Patch* sample);
    double computeSimilarity(Patch* templatePatch, Patch* samplePatch);

    double computeRelativeScore(Patch* sample);
    double computeConservativeScore(Patch* sample);
    ObjectScore* computeScore(Patch* patch);
    void add(Patch* patch, bool label);
    void replace(Patch* patch, int oldIndex);
};
#endif
