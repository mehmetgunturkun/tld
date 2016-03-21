#ifndef DETECTOR_HPP
#define DETECTOR_HPP

#include "core/Frame.hpp"
#include "core/Box.hpp"
#include "detector/DetectResult.hpp"
#include "detector/common/TrainingSet.hpp"
#include "detector/BoxIterator.hpp"

class CascadedSingleDetector {
public:
    void init(Frame* frame, Box* box);
    DetectResult* detect(Frame* frame);
    double score(Frame* frame, Box* box);
    void learn(Frame* frame, Box* box, DetectResult* detectResult);
};
#endif
