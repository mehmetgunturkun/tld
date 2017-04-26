#ifndef DETECTOR_RESULT_H
#define DETECTOR_RESULT_H

#include <vector>
#include "core/ScoredBox.hpp"

using namespace std;

class DetectorResult {
public:
    bool isValid;
    vector<ScoredBox*> allBoxList;
    vector<ScoredBox*> highVarianceBoxList;
    vector<ScoredBox*> candidateBoxList;
    vector<ScoredBox*> detectedBoxList;
    vector<ScoredBox*> clusteredBoxList;

    DetectorResult(vector<ScoredBox*> allBoxList,
                   vector<ScoredBox*> candidateBoxList,
                   vector<ScoredBox*> detectedBoxList,
                   vector<ScoredBox*> clusteredBoxList,
                   int modelId);
   ~DetectorResult();
};
#endif
