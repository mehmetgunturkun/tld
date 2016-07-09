#include "detector/variance/VarianceClassifier.hpp"

VarianceClassifier::VarianceClassifier(Frame* frame, vector<Box*> boxList) {
    minimumVariance = INT_MAX;
    for (int i = 0; i < boxList.size(); i++) {
            Box* b = boxList[i];
            MeanVariance* initialMeanVariance = frame->integral->computeMeanVariance(
                (int) b->x1,
                (int) b->y1,
                (int) b->width,
                (int) b->height
            );
            if (minimumVariance > initialMeanVariance->variance) {
                minimumVariance = initialMeanVariance->variance;
            }
    }
    minimumVariance = minimumVariance * 0.5;
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
