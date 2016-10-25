#ifndef VARIANCE_CLASSIFIER_H
#define VARIANCE_CLASSIFIER_H

#include "core/Frame.hpp"
#include "core/Box.hpp"
#include "core/ScoredBox.hpp"
#include "detector/variance/VarianceScore.hpp"

using namespace std;

class VarianceClassifier {
public:
    bool initiated;
    int nrOfModels;
    float minimumVariance;
    vector<float> varianceList;

    VarianceClassifier();
    VarianceClassifier(double minVariance);

    void setMinimumVariance(int modelId, double minVariance);

    void init(Frame* frame, vector<Box*> boxList);
    bool classify(Frame* frame, ScoredBox *scoredBox);
};
#endif
