#ifndef NEAREST_NEIGHBOR_H
#define NEAREST_NEIGHBOR_H

#include "core/Frame.hpp"
#include "core/Box.hpp"
#include "detector/common/ScoredBox.hpp"
#include "detector/common/TrainingSet.hpp"
#include "detector/nn/Patch.hpp"
#include "detector/nn/ObjectModel.hpp"

class NearestNeighborClassifier {
private:
    ObjectModel* model;
    double minimumPositiveThreshold;
public:
    NearestNeighborClassifier();
    void init(TrainingSet<Box> ts);
    bool classify(Frame* frame, ScoredBox* box);
    double score(Frame* frame, Box* box);
    void update(TrainingSet<ScoredBox> ts);
};
#endif
