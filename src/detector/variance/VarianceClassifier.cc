#include "detector/variance/VarianceClassifier.hpp"

VarianceClassifier::VarianceClassifier(double minVariance) {
    minimumVariance = minVariance;
}

bool VarianceClassifier::classify(Frame *frame, Box *box) {
    MeanVariance* meanVariance = frame->integral->computeMeanVariance(
        (int) box->x1,
        (int) box->y1,
        (int) box->width,
        (int) box->height);

    box->mean = meanVariance->mean;
    box->variance = meanVariance->variance;
    return box->variance > minimumVariance;
}
