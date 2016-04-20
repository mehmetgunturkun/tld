#include "detector/DetectResult.hpp"

DetectResult::DetectResult() {
    isFailed = false;
    allSize = 0;
    detectedSize = 0;
}

DetectResult::DetectResult(vector<ScoredBox*> a, vector<ScoredBox*> d) {
    all = a;
    allSize = (int) all.size();

    detected = d;
    detectedSize = (int) detected.size();

    isFailed = detectedSize == 0;
}
