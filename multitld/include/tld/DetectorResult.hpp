#ifndef DETECTOR_RESULT_H
#define DETECTOR_RESULT_H

class DetectorResult {
public:
    bool isValid;
    bool hasOnlyOneBox;

    vector<ScoredBox*> allBoxList;
    vector<ScoredBox*> candidateBoxList;
    vector<ScoredBox*> detectedBoxList;
    vector<ScoredBox*> clusteredBoxList;

    DetectorResult(vector<ScoredBox*> allBoxList, int modelId) {
        int nrOfBoxes = allBoxList.size();
        for (int i = 0; i < nrOfBoxes; i++) {
            ScoredBox* scoredBox = allBoxList[i];
            Score* varianceScore = scoredBox->getScore("variance");
            vector<int> candidateModelIds = varianceScore->classifiedModelIds;
            for (int i = 0; i < candidateModelIds.size(); i++) {
                int candidateModelId = candidateModelIds[i];
                if (modelId == candidateModelId) {
                    allBoxList.push_back(scoredBox);
                    if (scoredBox->isDetected) {
                        detectedBoxList.push_back(scoredBox);
                    }
                    break;
                }
            }
        }

        clusteredBoxList = ScoredBox::cluster(detectedBoxList, (int) detectedBoxList.size());
    }
};
#endif
