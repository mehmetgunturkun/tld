#ifndef NN_CLASSIFICATION_DETAILS_H
#define NN_CLASSIFICATION_DETAILS_H

#include "detector/common/ClassificationDetails.hpp"
#include "detector/nn/Patch.hpp"


class NNClassificationDetails: public ClassificationDetails {
public:
    Patch* patch;
    double score;

    NNClassificationDetails(Patch* p, double s) {
        this->patch = p;
        this->score = s;
    }
};
#endif
