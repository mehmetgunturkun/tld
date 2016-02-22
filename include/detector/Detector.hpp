#ifndef DETECTOR_HPP
#define DETECTOR_HPP

#include "core/Frame.hpp"
#include "core/Box.hpp"
#include "detector/DetectResult.hpp"

class Detector {
public:
    DetectResult* detect(Frame* frame) {
        throw "NotImplemented!";
    }

    double score(Frame* frame, Box* box) {
        throw "NotImplemented!";
    }

    void learn(Frame* frame, Box* box) {
        throw "NotImplemented!";
    }
};
#endif
