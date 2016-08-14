#ifndef VARIANCE_CLASSIFIER_H
#define VARIANCE_CLASSIFIER_H

#include "core/Frame.hpp"
#include "core/Box.hpp"
#include "core/ScoredBox.hpp"
#include "detector/variance/VarianceScore.hpp"

using namespace std;

class VarianceClassifier {
public:
    float minimumVariance;
    int nrOfModels;
    vector<float> varianceList;

    VarianceClassifier(Frame* frame, vector<Box*> boxList);
    VarianceClassifier(double minVariance);
    bool classify(Frame* frame, ScoredBox *scoredBox);
};
#endif
