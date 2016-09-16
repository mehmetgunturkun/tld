#ifndef DETECTOR_RESULT_H
#define DETECTOR_RESULT_H

class DetectorResult {
public:
    bool isValid;

    vector<ScoredBox*> highVarianceBoxList;
    vector<ScoredBox*> candidateBoxList;
    vector<ScoredBox*> detectedBoxList;
    vector<ScoredBox*> clusteredBoxList;

    DetectorResult(vector<ScoredBox*> allBoxList,
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
};
#endif
