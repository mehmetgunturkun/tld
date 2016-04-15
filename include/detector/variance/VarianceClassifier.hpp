#ifndef VARIANCE_CLASSIFIER_H
#define VARIANCE_CLASSIFIER_H

#include "core/Frame.hpp"
#include "core/Box.hpp"

class VarianceClassifier {
private:
    double minimumVariance;
public:
    VarianceClassifier(Frame* f, Box* b);
    VarianceClassifier(double minVariance);
    bool classify(Frame* frame, Box* box);
};
#endif
