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

vector<Box*> TLD::track(Frame* prev, Frame* curr, vector<Box*> prevBoxList) {
    vector<Box*> currentBoxList = tracker->track(prev, curr, prevBoxList);
    vector<ScoredBox*> scoredBoxList = detector->detect(curr);
    vector<TLDResultSet*> resultSetPerModel = groupResults(currentBoxList, scoredBoxList);

    vector<Box*> estimatedBoxList;
    for (int modelId = 0; modelId < nrOfModels; modelId++) {
        TLDResultSet* resultSet = resultSetPerModel[modelId];
        Option<Box>* maybeCurrentBox = integrate(curr, resultSet->maybeTrackedBox, resultSet->scoredBoxList, modelId);
        if (maybeCurrentBox->isDefined()) {
            Box* currentBox = maybeCurrentBox->get();
            estimatedBoxList.push_back(currentBox);
        } else {
            estimatedBoxList.push_back(nullptr);
            printf("No valid result for %d!\n", modelId);
        }
    }
    return estimatedBoxList;
}

vector<TLDResultSet*> TLD::groupResults(vector<Box*> boxList, vector<ScoredBox*> scoredBoxList) {
    vector<TLDResultSet*> resultSetPerModel;
    for (int i = 0; i < nrOfModels; i++) {
        Box* box = boxList[i];
        TLDResultSet* resultSet = new TLDResultSet(box);
        resultSetPerModel.push_back(resultSet);
    }

    int nrOfBoxes = scoredBoxList.size();
    for (int i = 0; i < nrOfBoxes; i++) {
        ScoredBox* scoredBox = scoredBoxList[i];
        Score* varianceScore = scoredBox->getScore("variance");

        vector<int> candidateModelIds = varianceScore->classifiedModelIds;
        int nrOfCandidateModels = (int) candidateModelIds.size();

        for (int j = 0; j < nrOfCandidateModels; j++) {
            int modelId = candidateModelIds[j];
            resultSetPerModel[modelId]->add(scoredBox);
        }
    }

    return resultSetPerModel;
}

void display(Frame* frame, vector<ScoredBox*> b1, vector<ScoredBox*> b2, vector<ScoredBox*> b3) {
    ImageBuilder* builder = new ImageBuilder(frame);

    int nrOfBoxes = (int) b1.size();
    for (int i = 0; i < nrOfBoxes; i++) {
        ScoredBox* b = b1[i];
        if (b != nullptr) {
            builder->withBox(b->box, Colors::RED)->withTitle("detection");
        }
    }

    nrOfBoxes = (int) b2.size();
    for (int i = 0; i < nrOfBoxes; i++) {
        ScoredBox* b = b2[i];
        if (b != nullptr) {
            builder->withBox(b->box, Colors::YELLOW)->withTitle("detection");
        }
    }

    nrOfBoxes = (int) b3.size();
    for (int i = 0; i < nrOfBoxes; i++) {
        ScoredBox* b = b3[i];
        if (b != nullptr) {
            builder->withBox(b->box, Colors::BLUE)->withTitle("detection");
        }
    }

    builder->display(1);
}

Option<Box>* TLD::integrate(Frame* frame, Box* maybeTrackedBox, vector<ScoredBox*> scoredBoxList, int modelId) {
    // Validation
    TrackerResult* trackerResult = validate(frame, maybeTrackedBox, modelId);
    ScoredBox* scoredTrackBox = trackerResult->scoredBox;

    DetectorResult* detectorResult = partition(scoredBoxList, modelId);
    vector<ScoredBox*> highVarianceBoxList = detectorResult->highVarianceBoxList;
    vector<ScoredBox*> candidateBoxList = detectorResult->candidateBoxList;
    vector<ScoredBox*> detectedBoxList = detectorResult->detectedBoxList;
    vector<ScoredBox*> clusteredBoxList = detectorResult->clusteredBoxList;

    //display(frame, candidateBoxList, detectedBoxList, clusteredBoxList);

    bool shouldLearn = false;
    Option<Box>* maybeFinalBox = Box::None;

    // Integration
    if (trackerResult->isValid) {
        printf("Frame#%d, Tracker " ANSI_COLOR_GREEN "Success(%f) " ANSI_COLOR_RESET " \n", frame->id);
        shouldLearn = scoredTrackBox->isDetected;
        if (detectedBoxList.size() > 0) {
            vector<ScoredBox*> moreConfidentBoxList = getMoreConfidentBoxList(scoredTrackBox, clusteredBoxList);
            if (moreConfidentBoxList.size() == 1) {
                // Detector.Override
                printf("Detector " ANSI_COLOR_GREEN "Success Override " ANSI_COLOR_RESET " \n");
                ScoredBox* detectedBox = moreConfidentBoxList[0];
                maybeFinalBox = new Option<Box>(detectedBox->box);
            } else {
                // Detector.Combine
                printf("Detector " ANSI_COLOR_GREEN "Success Combine " ANSI_COLOR_RESET " \n");
                Box* combinedBox = combineClosestBoxes(scoredTrackBox, candidateBoxList);
                maybeFinalBox = new Option<Box>(combinedBox);
                // detector->learn(current, combinedBox, allBoxes, modelId);
                shouldLearn = shouldLearn && true;
            }
        } else {
            printf("Detector " ANSI_COLOR_RED "Failed " ANSI_COLOR_RESET " \n");
            // Detector.Fail
            maybeFinalBox = new Option<Box>(scoredTrackBox->box);
            // detector->learn(current, trackedBox, allBoxes, modelId);
        }
    } else {
        // Tracker.Fail
        printf("Tracker " ANSI_COLOR_RED "Failed " ANSI_COLOR_RESET " \n");
        if (detectedBoxList.size() > 0) {
            if (clusteredBoxList.size() == 1) {
                printf("Detector " ANSI_COLOR_GREEN "Success One " ANSI_COLOR_RESET " \n");
                // Detector.Success
                ScoredBox* detectedBox = clusteredBoxList[0];
                maybeFinalBox = new Option<Box>(detectedBox->box);
            } else if (clusteredBoxList.size() > 1) {
                printf("Detector " ANSI_COLOR_RED "Failed Too Many Items " ANSI_COLOR_RESET " \n");
                // There are multiple boxes no way to decide!
                maybeFinalBox = Box::None;
            } else {
                printf("Detector " ANSI_COLOR_RED " Should not happen " ANSI_COLOR_RESET " \n");
                // Detector.Fail
                maybeFinalBox = Box::None;
            }
        } else {
            printf("Detector " ANSI_COLOR_RED "Failed " ANSI_COLOR_RESET " \n");
            // Detector.Fail
            maybeFinalBox = Box::None;
        }
    }

    // Evaluation
    if (maybeFinalBox->isDefined()) {
        Box* finalBox = maybeFinalBox->get();
        bool isValid = detector->evaluate(frame, finalBox, modelId);

        if (isValid) {
            printf("Evaluation" ANSI_COLOR_GREEN " Success " ANSI_COLOR_RESET " \n");
        } else {
            printf("Evaluation" ANSI_COLOR_RED " Failed " ANSI_COLOR_RESET " \n");
            shouldLearn = false;
        }
    } else {
        shouldLearn = false;
    }

    // Learning
    if (shouldLearn) {
        Box* finalBox = maybeFinalBox->get();
        detector->learn(frame, finalBox, scoredBoxList, modelId);
        printf("Learner " ANSI_COLOR_GREEN "Success" ANSI_COLOR_RESET " \n");
    } else {
        printf("Learner " ANSI_COLOR_RED "No Need" ANSI_COLOR_RESET " \n");
    }

    return maybeFinalBox;
};

TrackerResult* TLD::validate(Frame* current, Box* trackedBox, int modelId) {
    if (trackedBox == nullptr) {
        return new TrackerResult();
        // return new ScoredBox(trackedBox);
    } else {
        ScoredBox* scoredBox = detector->validate(current, trackedBox, modelId);
        return new TrackerResult(scoredBox);
    }
}

DetectorResult* TLD::partition(vector<ScoredBox*> scoredBoxList, int modelId) {
    vector<ScoredBox*> candidateBoxList;
    vector<ScoredBox*> detectedBoxList;

    int nrOfBoxes = scoredBoxList.size();
    for (int i = 0; i < nrOfBoxes; i++) {
        ScoredBox* scoredBox = scoredBoxList[i];
        if (scoredBox->isClassified("ensemble", modelId)) {
            candidateBoxList.push_back(scoredBox);
        }

        if (scoredBox->isClassified("nn", modelId)) {
            detectedBoxList.push_back(scoredBox);
        }
    }
    vector<ScoredBox*> clusteredBoxList = ScoredBox::cluster(detectedBoxList, (int) detectedBoxList.size());

    if ((int) detectedBoxList.size() > 0 && clusteredBoxList.size() == 0) {
        printf(ANSI_COLOR_RED "What The Fuck!\n" ANSI_COLOR_RESET);
    }

    return new DetectorResult(scoredBoxList, candidateBoxList, detectedBoxList, clusteredBoxList, modelId);
}

vector<ScoredBox*> TLD::getMoreConfidentBoxList(ScoredBox* trackScoredBox, vector<ScoredBox*> detectedBoxes) {
    float minOverlap = 0.5;
    vector<ScoredBox*> moreConfidentBoxList;
    int nrOfBoxes = (int) detectedBoxes.size();

    double trackScore = trackScoredBox->getScoreValue("nn");
    Box* trackBox = trackScoredBox->box;

    for (int i = 0; i < nrOfBoxes; i++) {
        ScoredBox* scoredBox = detectedBoxes[i];
        double detectScore = scoredBox->getScoreValue("nn");;
        float overlap = Box::computeOverlap(trackBox, scoredBox->box);
        if (overlap < minOverlap && detectScore > trackScore) {
            moreConfidentBoxList.push_back(scoredBox);
        }
    }

    return moreConfidentBoxList;
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
