#ifndef NEAREST_NEIGHBOR_CLASSIFIER_H
#define NEAREST_NEIGHBOR_CLASSIFIER_H

#include <vector>

#include "core/Frame.hpp"
#include "core/ScoredBox.hpp"

#include "detector/training/TrainingSet.hpp"

#include "detector/nn/Patch.hpp"
#include "detector/nn/NNScore.hpp"
#include "detector/nn/ObjectModel.hpp"


using namespace std;

class NearestNeighborClassifier {
private:
    int nrOfModels;
    vector<ObjectModel*> models;
public:
    NearestNeighborClassifier();
    bool classify(Frame* frame, ScoredBox* scoredBox);
    void train(TrainingSet<Box> ts, int modelId);
    void train(TrainingSet<ScoredBox> ts, int modelId);
};
#endif
