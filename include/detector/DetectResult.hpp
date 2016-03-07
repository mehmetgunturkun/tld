#ifndef DETECTION_RESULT_HPP
#define DETECTION_RESULT_HPP

#include "core/Box.hpp"
#include "core/ScoredBox.hpp"

using namespace std;

class DetectResult {
public:
    bool isFailed;

    int allSize;
    vector<ScoredBox*> all;

    int detectedSize;
    vector<ScoredBox*> detected;
};
#endif
