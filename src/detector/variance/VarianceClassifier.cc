#include "detector/variance/VarianceClassifier.hpp"

VarianceClassifier::VarianceClassifier(Frame* f, Box* b) {
    MeanVariance* initialMeanVariance = f->integral->computeMeanVariance(
        (int) b->x1,
        (int) b->y1,
        (int) b->width,
        (int) b->height
    );
    minimumVariance = initialMeanVariance->variance * 0.5;
}

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
