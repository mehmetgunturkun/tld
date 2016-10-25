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
    int closestPositivePatchIndex;

    ObjectScore(double relativeScore, double conservativeScore, bool isInPositive, bool isInNegative, int closestPositivePatchIndex);
};

class ObjectModel {
public:
    int nrOfPositivePatches;
    vector<Patch*> positivePatchList;

    int nrOfNegativePatches;
    vector<Patch*> negativePatchList;

    ObjectModel();

    tuple<double, int> computeInitialPositiveSimilarity(Patch* sample);
    tuple<double, int> computePositiveSimilarity(Patch* sample);
    tuple<double, int> computeNegativeSimilarity(Patch* sample);
    tuple<double, int> computeNearestSimilarity(vector<Patch*> templatePatchList, int size, Patch* sample);
    double computeSimilarity(Patch* templatePatch, Patch* samplePatch);

    ObjectScore* computeScore(Patch* patch);

    void add(Patch* patch, bool label);
    void add(Patch* patch, int index, bool label);
    void replace(Patch* patch, int oldIndex);
};
#endif
