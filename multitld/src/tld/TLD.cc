#include "tld/TLD.hpp"

TLD::TLD(Frame* frame, vector<Box*> boxList) {
    this->nrOfModels = (int) boxList.size();

    this->tracker = new Tracker();
    this->detector = new Detector(frame, boxList);
}

TLD::TLD(Tracker* tracker, Detector* detector) {
    this->tracker = tracker;
    this->detector = detector;
    this->nrOfModels = 0;
}

vector<Box*> TLD::track(Frame* prev, Frame* curr, vector<Box*> boxList) {
    vector<Box*> currBoxList = tracker->track(prev, curr, boxList);

    vector<ScoredBox*> currScoredBoxList = detector->detect(curr);
    vector<vector<ScoredBox*>> groupedAllBoxes;
    vector<vector<ScoredBox*>> groupedScoredBoxes;

    for (int i = 0; i < nrOfModels; i++) {
        vector<ScoredBox*> allBoxPerModel;
        groupedAllBoxes.push_back(allBoxPerModel);

        vector<ScoredBox*> scoredBoxPerModel;
        groupedScoredBoxes.push_back(scoredBoxPerModel);
    }

    for (int i = 0; i < currScoredBoxList.size(); i++) {
        ScoredBox* scoredBox = currScoredBoxList[i];

        Score* varianceScore = scoredBox->getScore("variance");
        vector<int> candidateModelIds = varianceScore->classifiedModelIds;
        for (int i = 0; i < candidateModelIds.size(); i++) {
            int modelId = candidateModelIds[i];
            groupedAllBoxes[modelId].push_back(scoredBox);
        }

        if (scoredBox->isDetected) {
            vector<int> candidateModelIds = scoredBox->getCandidateModels();
            int modelId = candidateModelIds[0];
            groupedScoredBoxes[modelId].push_back(scoredBox);
        }
    }

    vector<Box*> estimatedBoxList;
    for (int i = 0; i < nrOfModels; i++) {
        Box* trackedBox = currBoxList[i];
        vector<ScoredBox*> allBoxList = groupedAllBoxes[i];
        vector<ScoredBox*> detectedBoxList = groupedScoredBoxes[i];

        Option<Box>* maybeBox = integrate(curr, trackedBox, allBoxList, detectedBoxList, i);
        if (maybeBox->isDefined()) {
            estimatedBoxList.push_back(maybeBox->get());
        } else {
            estimatedBoxList.push_back(nullptr);
            printf("No valid result for %d!\n", i);
        }
    }
    return estimatedBoxList;
}

void displayDetectedBoxes(Frame* frame, vector<ScoredBox*> detectedBoxList, vector<ScoredBox*> clusteredBoxList) {
    ImageBuilder* builder = new ImageBuilder(frame);

    int nrOfBoxes = (int) detectedBoxList.size();
    for (int i = 0; i < nrOfBoxes; i++) {
        ScoredBox* b = detectedBoxList[i];
        if (b != nullptr) {
            builder->withBox(b->box, Colors::RED);
        }
    }

    nrOfBoxes = (int) clusteredBoxList.size();
    for (int i = 0; i < nrOfBoxes; i++) {
        ScoredBox* b = clusteredBoxList[i];
        if (b != nullptr) {
            builder->withBox(b->box, Colors::BLUE);
        }
    }


    builder->withTitle("cluster-test")->display(100);
}

Option<Box>* TLD::integrate(Frame* current, Box* trackedBox, vector<ScoredBox*> allBoxes, vector<ScoredBox*> detectedBoxes, int modelId) {
    ScoredBox* scoredTrackBox = validate(current, trackedBox, modelId);
    vector<ScoredBox*> clusteredBoxes = ScoredBox::cluster(detectedBoxes,(int) detectedBoxes.size());

    //displayDetectedBoxes(current, detectedBoxes, clusteredBoxes);

    printf("Frame#%3d => ", current->id);
    if (scoredTrackBox->isDetected) {
        printf("Tracker " ANSI_COLOR_GREEN "Success " ANSI_COLOR_RESET);
        // Tracker.Success
        if (detectedBoxes.size() > 0) {
            if (isThereMoreConfidentOneBox(scoredTrackBox, clusteredBoxes)) {
                printf("Detector " ANSI_COLOR_GREEN "Success One" ANSI_COLOR_RESET "\n");
                ScoredBox* detectedBox = clusteredBoxes[0];
                Option<Box>* successBox = new Option<Box>(detectedBox->box);
                return successBox;
            } else {
                printf("Detector " ANSI_COLOR_GREEN "Success Combine" ANSI_COLOR_RESET "\n");
                Box* combinedBox = combineClosestBoxes(scoredTrackBox, clusteredBoxes);
                detector->learn(current, combinedBox, allBoxes, modelId);
                Option<Box>* successBox = new Option<Box>(combinedBox);
                return successBox;
            }
        } else {
            printf("Detector " ANSI_COLOR_RED "Failed " ANSI_COLOR_RESET "\n");
            // Detector.Fail
            Option<Box>* successBox = new Option<Box>(trackedBox);
            detector->learn(current, trackedBox, allBoxes, modelId);
            return successBox;
        }
    } else {
        // Tracker.Fail
        printf("Tracker " ANSI_COLOR_RED "Failed " ANSI_COLOR_RESET);
        if (clusteredBoxes.size() == 1) {
            printf("Detector " ANSI_COLOR_GREEN "Success " ANSI_COLOR_RESET" \n");
            // Detector.Success
            ScoredBox* detectedBox = clusteredBoxes[0];
            Option<Box>* successBox = new Option<Box>(detectedBox->box);
            return successBox;
        } else if (detectedBoxes.size() > 1) {
            printf("Detector " ANSI_COLOR_GREEN "Success " ANSI_COLOR_RESET" \n");
            // There are multiple boxes no way to decide!
            return Box::None;
        } else {
            printf("Detector " ANSI_COLOR_GREEN "Success " ANSI_COLOR_RESET" \n");
            // Detector.Fail
            return Box::None;
        }
    }
}

ScoredBox* TLD::validate(Frame* current, Box* trackedBox, int modelId) {
    float minValidationScore = 0.0f;
    if (trackedBox == nullptr) {
        return new ScoredBox(trackedBox);
    } else {
        ScoredBox* scoredBox = detector->validate(current, trackedBox, modelId);
        if (scoredBox->isDetected) {
            return scoredBox;
        } else {
            printf("Tracker Invalid ");
            return new ScoredBox(trackedBox);
        }
    }
}

bool TLD::isThereMoreConfidentOneBox(ScoredBox* trackScoredBox, vector<ScoredBox*> detectedBoxes) {
    float minOverlap = 0.0;

    int detectedSize = (int) detectedBoxes.size();
    if (detectedSize == 1) {
        bool moreConfident = false;

        double trackScore = trackScoredBox->getScoreValue("nn");
        Box* trackBox = trackScoredBox->box;

        for (int i = 0; i < detectedSize; i++) {
            ScoredBox* scoredBox = detectedBoxes[i];
            double detectScore = scoredBox->getScoreValue("nn");;
            float overlap = Box::computeOverlap(trackBox, scoredBox->box);
            if (overlap < minOverlap && detectScore > trackScore) {
                moreConfident = true;
                break;
            }
        }
        return moreConfident;
    } else {
        return false;
    }
}

Box* TLD::combineClosestBoxes(ScoredBox* trackScoredBox, vector<ScoredBox*> detectedBoxes) {
    Box* trackBox = trackScoredBox->box;

    float x1 = trackBox->x1*10;
    float y1 = trackBox->y1*10;
    float x2 = trackBox->x2*10;
    float y2 = trackBox->y2*10;
    int patchCount = 10;

    int patchSize = (int) detectedBoxes.size();
    for (int i = 0; i < patchSize; i++) {
        ScoredBox* scoredBox = detectedBoxes[i];
        Box* box = scoredBox->box;
        float overlap = Box::computeOverlap(trackBox, box);
        if (overlap > 0.7) {
            x1 += box->x1;
            y1 += box->y1;
            x2 += box->x2;
            y2 += box->y2;

            patchCount++;
        }
    }

    x1 /= patchCount;
    y1 /= patchCount;
    x2 /= patchCount;
    y2 /= patchCount;

    Box* meanBox = new Box(0, x1, y1, x2, y2);
    return meanBox;
}
