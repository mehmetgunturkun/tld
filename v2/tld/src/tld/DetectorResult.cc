#include "tld/DetectorResult.hpp"

DetectorResult::DetectorResult(vector<ScoredBox*> allBoxList,
               vector<ScoredBox*> candidateBoxList,
               vector<ScoredBox*> detectedBoxList,
               vector<ScoredBox*> clusteredBoxList,
               int modelId) {
    this->isValid = detectedBoxList.size() > 0;
    this->highVarianceBoxList = allBoxList;
    this->candidateBoxList = candidateBoxList;
    this->detectedBoxList = detectedBoxList;
    this->clusteredBoxList = clusteredBoxList;
}
