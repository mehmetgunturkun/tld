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

DetectorResult::~DetectorResult() {
    int nrOfClusters = (int) clusteredBoxList.size();
    for (int i = 0; i < nrOfClusters; i++) {
        ScoredBox* scoredBox = clusteredBoxList[i];
        delete scoredBox;
    }
}
