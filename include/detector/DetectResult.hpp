#ifndef DETECTION_RESULT_HPP
#define DETECTION_RESULT_HPP

#include <vector>

#include "core/Box.hpp"
#include "detector/common/ScoredBox.hpp"

using namespace std;

class DetectResult {
public:
    bool isFailed;

    int allSize;
    vector<ScoredBox*> all;

    int detectedSize;
    vector<ScoredBox*> detected;

    DetectResult();    
    DetectResult(vector<ScoredBox*> a, vector<ScoredBox*> d);
};
#endif
