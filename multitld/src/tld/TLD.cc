#include "tld/TLD.hpp"

TLD::TLD(Frame* frame, vector<Box*> boxList) {
    this->firstFrame = frame;
    this->firstBoxList = boxList;

    this->nrOfModels = (int) boxList.size();

    this->tracker = new Tracker();
    this->detector = new Detector(frame, boxList);
    printf("TLD is created\n");
}

vector<Box*> TLD::init() {
    vector<Box*> correctedBox = this->detector->init(this->firstFrame, this->firstBoxList);
    return correctedBox;
}

TLD::TLD(Tracker* tracker, Detector* detector) {
    this->tracker = tracker;
    this->detector = detector;
    this->nrOfModels = 0;
}

vector<Box*> TLD::track(Frame* prev, Frame* curr, vector<Box*> prevBoxList) {
    printf(GREEN("==== Tracker is started ====\n"));
    vector<Box*> currentBoxList = tracker->track(prev, curr, prevBoxList);
    printf(GREEN("==== Tracker is completed ====\n"));

    printf(RED("==== Detector is started ====\n"));
    vector<ScoredBox*> scoredBoxList = detector->detect(curr);
    printf(RED("==== Detector is completed ====\n"));

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
            builder->withBox(b->box, Colors::YELLOW);
        }
    }

    nrOfBoxes = (int) b2.size();
    for (int i = 0; i < nrOfBoxes; i++) {
        ScoredBox* b = b2[i];
        if (b != nullptr) {
            builder->withBox(b->box, Colors::RED);
        }
    }

    nrOfBoxes = (int) b3.size();
    for (int i = 0; i < nrOfBoxes; i++) {
        ScoredBox* b = b3[i];
        if (b != nullptr) {
            builder->withBox(b->box, Colors::BLUE);
        }
    }

    builder->withTitle("detection")->display(1);
}

Option<Box>* TLD::integrate(Frame* frame, Box* maybeTrackedBox, vector<ScoredBox*> scoredBoxList, int modelId) {
    // Validation
    printf(BOLD(WHITE("==== Integrate is started ====\n")));
    TrackerResult* trackerResult = validate(frame, maybeTrackedBox, modelId);
    ScoredBox* scoredTrackBox = trackerResult->scoredBox;

    DetectorResult* detectorResult = partition(scoredBoxList, modelId);
    vector<ScoredBox*> highVarianceBoxList = detectorResult->highVarianceBoxList;
    vector<ScoredBox*> candidateBoxList = detectorResult->candidateBoxList;
    vector<ScoredBox*> detectedBoxList = detectorResult->detectedBoxList;
    vector<ScoredBox*> clusteredBoxList = detectorResult->clusteredBoxList;

    display(frame, candidateBoxList, detectedBoxList, clusteredBoxList);

    bool shouldLearn = false;
    Option<Box>* maybeFinalBox = Box::None;

    // Integration
    if (trackerResult->isValid) {
        shouldLearn = scoredTrackBox->isDetected;
        if (detectedBoxList.size() > 0) {
            vector<ScoredBox*> moreConfidentBoxList = getMoreConfidentBoxList(scoredTrackBox, clusteredBoxList, modelId);
            if (moreConfidentBoxList.size() == 1) {
                printf("Tracker OK, Detector OVERRIDE\n");
                ScoredBox* detectedBox = moreConfidentBoxList[0];
                // ImageBuilder* builder = new ImageBuilder(frame);
                // builder->withBox(detectedBox->box, Colors::BLUE)->withTitle("override")->display(0);
                maybeFinalBox = new Option<Box>(detectedBox->box);
            } else {
                // Detector.Combine
                printf("Tracker OK, Detector COMBINE\n");
                Box* combinedBox = combineClosestBoxes(scoredTrackBox, candidateBoxList);
                maybeFinalBox = new Option<Box>(combinedBox);
                shouldLearn = shouldLearn && true;
            }
        } else {
            // Detector.Fail
            printf("Tracker OK, Detector FAILED\n");
            maybeFinalBox = new Option<Box>(scoredTrackBox->box);
        }
    } else {
        // Tracker.Fail
        if (detectedBoxList.size() > 0) {
            if (clusteredBoxList.size() == 1) {
                printf("Tracker FAIL, Detector ONE-BOX\n");
                // Detector.Success
                ScoredBox* detectedBox = clusteredBoxList[0];
                maybeFinalBox = new Option<Box>(detectedBox->box);
            } else if (clusteredBoxList.size() > 1) {
                // There are multiple boxes no way to decide!
                printf("Tracker FAIL, Detector MULTIPLE-BOX\n");
                maybeFinalBox = Box::None;
            } else {
                // Detector.Fail
                printf("Tracker FAIL, Detector FAIL (SHOULDN'T HAVE HAPPENED)\n");
                maybeFinalBox = Box::None;
            }
        } else {
            // Detector.Fail
            printf("Tracker FAIL, Detector FAIL\n");
            maybeFinalBox = Box::None;
        }
    }

    printf(BOLD(WHITE("==== Integrate is completed ====\n")));

    // Evaluation
    printf(CYAN("==== Evaluate is started ====\n"));
    // printf("Evaluation" COLOR_GREEN " Started " COLOR_RESET " \n");
    if (maybeFinalBox->isDefined()) {
        Box* finalBox = maybeFinalBox->get();
        bool isValid = detector->evaluate(frame, finalBox, modelId);

        if (isValid) {
            // printf("Evaluation" COLOR_GREEN " Success " COLOR_RESET " \n");
        } else {
            // printf("Evaluation" COLOR_RED " Failed " COLOR_RESET " \n");
            shouldLearn = false;
        }
    } else {
        shouldLearn = false;
    }
    printf(CYAN("==== Evaluate is completed ====\n"));

    // Learning
    printf(YELLOW("==== Learner is started ====\n"));
    if (shouldLearn) {
        Box* finalBox = maybeFinalBox->get();
        detector->learn(frame, finalBox, scoredBoxList, modelId);
        // printf("Learner " COLOR_GREEN "Success" COLOR_RESET " \n");
    } else {
        // printf("Learner " COLOR_RED "No Need" COLOR_RESET " \n");
    }
    printf(YELLOW("==== Learner is completed ====\n"));

    return maybeFinalBox;
};

TrackerResult* TLD::validate(Frame* current, Box* trackedBox, int modelId) {
    if (trackedBox == nullptr) {
        return new TrackerResult();
        // return new ScoredBox(trackedBox);
    } else {
        printf("%s\n", trackedBox->toCharArr());
        detector->validate(current, trackedBox, 0);
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

    for (int i = 0; i < candidateBoxList.size(); i++) {
        printf("EC >> %s\n", candidateBoxList[i]->box->toCharArr());
    }

    for (int i = 0; i < detectedBoxList.size(); i++) {
        printf("NN >> %s\n", detectedBoxList[i]->box->toCharArr());
    }

    for (int i = 0; i < clusteredBoxList.size(); i++) {
        printf("CB >> %s, %f\n", clusteredBoxList[i]->box->toCharArr(), clusteredBoxList[i]->getScoreValue("nn", 0));
    }

    return new DetectorResult(scoredBoxList, candidateBoxList, detectedBoxList, clusteredBoxList, modelId);
}

vector<ScoredBox*> TLD::getMoreConfidentBoxList(ScoredBox* trackScoredBox, vector<ScoredBox*> detectedBoxes, int modelId) {
    float minOverlap = 0.5;
    vector<ScoredBox*> moreConfidentBoxList;
    int nrOfBoxes = (int) detectedBoxes.size();

    float trackScore = trackScoredBox->getScoreValue("nn", modelId);
    Box* trackBox = trackScoredBox->box;

    for (int i = 0; i < nrOfBoxes; i++) {
        ScoredBox* scoredBox = detectedBoxes[i];
        float detectScore = scoredBox->getScoreValue("nn", modelId);

        float overlap = Box::computeOverlap(trackBox, scoredBox->box);
        if (overlap < minOverlap && detectScore > trackScore) {
            moreConfidentBoxList.push_back(scoredBox);
        }
    }

    return moreConfidentBoxList;
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
