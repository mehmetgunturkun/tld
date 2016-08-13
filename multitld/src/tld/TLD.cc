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

    for (int i = 0; i < currBoxList.size(); i++) {
        Box* box = currBoxList[i];
        ImageBuilder* builder = new ImageBuilder();
        builder->
            withFrame(curr)->
            withBox(box, Colors::YELLOW)
            ->display(50);
    }

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

    for (int i = 0; i < NR_OF_MODELS; i++) {
        vector<ScoredBox*> scoredBoxesPerModel = groupedScoredBoxes[i];
        Scalar color = Colors::colors[i];
        for (int j = 0; j < scoredBoxesPerModel.size(); j++) {
            ScoredBox* scoredBox = scoredBoxesPerModel[j];
            ImageBuilder* builder = new ImageBuilder();
            builder->
                withFrame(curr)->
                withBox(scoredBox->box, color)
                ->display(50);
        }
    }

    return currBoxList;
}
