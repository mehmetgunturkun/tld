#include "tld/TLD.hpp"

TLD::TLD(Frame* frame, vector<Box*> boxList) {
    this->tracker = new Tracker();
    this->detector = new Detector(frame, boxList);
    this->detector->init(frame, boxList);
}

TLD::TLD(Tracker* tracker, Detector* detector) {
    this->tracker = tracker;
    this->detector = detector;
}

vector<Box*> TLD::track(Frame* prev, Frame* curr, vector<Box*> boxList) {
    vector<Box*> currBoxList = tracker->track(prev, curr, boxList);
    vector<ScoredBox*> currScoredBoxList = detector->detect(curr);

    vector<vector<ScoredBox*>> groupedScoredBoxes;
    for (int i = 0; i < NR_OF_MODELS; i++) {
        vector<ScoredBox*> scoredBoxPerModel;
        groupedScoredBoxes.push_back(scoredBoxPerModel);
    }

    for (int i = 0; i < currScoredBoxList.size(); i++) {
        ScoredBox* scoredBox = currScoredBoxList[i];
        if (scoredBox->isDetected) {
            vector<int> candidateModelIds = scoredBox->getCandidateModels();
            int modelId = candidateModelIds[0];
            groupedScoredBoxes[modelId].push_back(scoredBox);
        }
    }

    ImageBuilder* builder = new ImageBuilder(curr);
    for (int i = 0; i < NR_OF_MODELS; i++) {
        Scalar tColor = Colors::colors[i * 2];
        Scalar dColor = Colors::colors[i * 2 + 1];
        Box* trackedBox = currBoxList[i];
        vector<ScoredBox*> detectedBoxList = groupedScoredBoxes[i];
        printf("mc1000\n");

        printf("mc2000\n");

        for (int i = 0; i < detectedBoxList.size(); i++) {
            ScoredBox* scoredBox = detectedBoxList[i];
            builder->withBox(scoredBox->box, dColor);
        }

        if (trackedBox != nullptr) {
            builder->withBox(trackedBox, tColor);
        }
    }
    builder->display(1000);

    return currBoxList;
}
