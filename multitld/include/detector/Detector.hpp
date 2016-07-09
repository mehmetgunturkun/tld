#ifndef DETECTOR_H
#define DETECTOR_H

#include "core/Frame.hpp"
#include "core/Box.hpp"
#include "core/ScoredBox.hpp"

#include "core/BoxIterator.hpp"
#include "detector/variance/VarianceClassifier.hpp"

class Detector {
public:
    int maxScaleLimit;
    int minimumPatchSize;

    Box* firstBox;
    VarianceClassifier* vClassifier;

    Detector(Frame* frame, vector<Box*> boxList);

    vector<ScoredBox*> detect(Frame* frame);
};
#endif
