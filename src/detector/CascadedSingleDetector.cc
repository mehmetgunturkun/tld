#include "detector/CascadedSingleDetector.hpp"

void CascadedSingleDetector::init(Frame* frame, Box* box) {
    throw "NotImplemented!";
}

DetectResult* CascadedSingleDetector::detect(Frame* frame) {
    DetectResult* detectResult = new DetectResult();
    return detectResult;
}

double CascadedSingleDetector::score(Frame* frame, Box* box) {
    return 0.0;
}

void CascadedSingleDetector::learn(Frame* frame, Box* box, DetectResult* detectResult) {
    throw "NotImplemented!";
}
