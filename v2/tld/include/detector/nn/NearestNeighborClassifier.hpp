#ifndef NEAREST_NEIGHBOR_CLASSIFIER_H
#define NEAREST_NEIGHBOR_CLASSIFIER_H

#include <vector>

#include "common/Logging.hpp"
#include "core/Frame.hpp"
#include "core/ScoredBox.hpp"
#include "core/util/ImageBuilder.hpp"

#include "detector/training/TrainingSet.hpp"

#include "detector/nn/Patch.hpp"
#include "detector/nn/NNScore.hpp"
#include "detector/nn/ObjectModel.hpp"


using namespace std;

class NearestNeighborClassifier {
private:
    int nrOfModels;
    vector<ObjectModel*> models;
    double POSITIVE_SCORE_THRESHOLD;
public:
    NearestNeighborClassifier();
    ~NearestNeighborClassifier();
    
    void init(Frame* firstFrame, vector<Box*> boxList);
    double getPatchVariance(Frame* frame, Box* box);

    bool classify(Frame* frame, ScoredBox* scoredBox);
    void score(Frame* frame, ScoredBox* scoredBox);

    bool evaluate(Frame* frame, Box* box, double minVariance, int modelId);
    bool validate(Frame* frame, ScoredBox* scoredBox, int modelId);

    void train(TrainingSet<ScoredBox> ts, int modelId);
    void dumpNearestNeighborClassifier();

    // v2
    void train(TrainingSet<Box> ts, int modelId);
    void doTrain(vector<Labelled<Patch>*> samples, int modelId);
    vector<Labelled<Patch>*> generateSamples(Frame* frame, vector<Box*> positiveBoxList, vector<Box*> negativeBoxList);
    vector<Labelled<Patch>*> generateSamples(Frame* frame, vector<ScoredBox*> positiveBoxList, vector<ScoredBox*> negativeBoxList);
};
#endif
