#ifndef VARIANCE_CLASSIFIER_H
#define VARIANCE_CLASSIFIER_H

#include "core/Frame.hpp"
#include "core/Box.hpp"

using namespace std;

class VarianceClassifier {
public:
    float minimumVariance;

    VarianceClassifier(Frame* frame, vector<Box*> boxList);
    VarianceClassifier(double minVariance);
    bool classify(Frame* frame, Box* box);
};
#endif
