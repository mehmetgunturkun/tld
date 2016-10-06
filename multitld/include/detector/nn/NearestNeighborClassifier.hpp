#ifndef NEAREST_NEIGHBOR_CLASSIFIER_H
#define NEAREST_NEIGHBOR_CLASSIFIER_H

#include <vector>

#include "core/Frame.hpp"
#include "core/ScoredBox.hpp"

#include "detector/training/TrainingSet.hpp"

#include "detector/nn/Patch.hpp"
#include "detector/nn/NNScore.hpp"
#include "detector/nn/ObjectModel.hpp"

#include "util/ImageBuilder.hpp"

using namespace std;

class NearestNeighborClassifier {
private:
    int nrOfModels;
    vector<ObjectModel*> models;
    float POSITIVE_SCORE_THRESHOLD;
public:
    NearestNeighborClassifier();
    void init(Frame* firstFrame, vector<Box*> boxList);
    bool classify(Frame* frame, ScoredBox* scoredBox);
    void score(Frame* frame, ScoredBox* scoredBox);

    bool evaluate(Frame* frame, Box* box, int modelId);
    bool validate(Frame* frame, ScoredBox* scoredBox, int modelId);

    void train(TrainingSet<Box> ts, int modelId);
    void train(TrainingSet<ScoredBox> ts, int modelId);
    void dumpNearestNeighborClassifier();
};
#endif
