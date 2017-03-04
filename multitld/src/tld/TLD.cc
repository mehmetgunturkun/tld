#include "tld/TLD.hpp"

TLD::TLD() {
    this->tracker = new Tracker();
    this->detector = new Detector();
}

TLD::TLD(Tracker* tracker, Detector* detector) {
    this->tracker = tracker;
    this->detector = detector;
    this->nrOfModels = 0;
}

vector<Box*> TLD::init(Frame* frame, vector<Box*> boxList) {
    this->firstFrame = frame;
    this->firstBoxList = boxList;

    this->nrOfModels = (int) boxList.size();

    vector<Box*> correctedBox = detector->init(firstFrame, firstBoxList);
    return correctedBox;
}

vector<Box*> TLD::track(Frame* prev, Frame* curr, vector<Box*> prevBoxList) {
    DEBUG(GREEN("==== Tracker is started ===="));
    vector<Box*> currentBoxList = tracker->track(prev, curr, prevBoxList);
    DEBUG(GREEN("==== Tracker is completed ===="));

    DEBUG(RED("==== Detector is started ===="));
    vector<ScoredBox*> scoredBoxList = detector->detect(curr);
    DEBUG(RED("==== Detector is completed ===="));

    vector<TLDResultSet*> resultSetPerModel = groupResults(currentBoxList, scoredBoxList);

    vector<Box*> estimatedBoxList;
    for (int modelId = 0; modelId < nrOfModels; modelId++) {
        TLDResultSet* resultSet = resultSetPerModel[modelId];
        Box* prevBox = prevBoxList[modelId];
        Option<Box>* maybeCurrentBox = integrate(curr, prevBox, resultSet->maybeTrackedBox, scoredBoxList, modelId);
        if (maybeCurrentBox->isDefined()) {
            Box* currentBox = maybeCurrentBox->get();
            printf("TLD >> %s\n", currentBox->toCharArr());
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

Option<Box>* TLD::integrate(Frame* frame, Box* oldBox, Box* maybeTrackedBox, vector<ScoredBox*> scoredBoxList, int modelId) {
    // Validation
    DEBUG(BOLD(WHITE("==== Integrate is started ====\n")));
    Option<ScoredBox>* maybeScoredBox = validate2(frame, oldBox, maybeTrackedBox, modelId);
    DetectorResult* detectorResult = partition(scoredBoxList, modelId);
    vector<ScoredBox*> highVarianceBoxList = detectorResult->highVarianceBoxList;
    vector<ScoredBox*> candidateBoxList = detectorResult->candidateBoxList;
    vector<ScoredBox*> detectedBoxList = detectorResult->detectedBoxList;
    vector<ScoredBox*> clusteredBoxList = detectorResult->clusteredBoxList;

    // display(frame, candidateBoxList, detectedBoxList, clusteredBoxList);

    bool shouldLearn = false;
    Option<Box>* maybeFinalBox = Box::None;

    // Integration
    if (maybeScoredBox->isDefined()) {
        ScoredBox* scoredTrackBox = maybeScoredBox->get();
        shouldLearn = scoredTrackBox->box->isValid;

        if (detectedBoxList.size() > 0) {
            vector<ScoredBox*> moreConfidentBoxList = getMoreConfidentBoxList(scoredTrackBox, clusteredBoxList, modelId);
            if (moreConfidentBoxList.size() == 1) {
                printf("Tracker OK, Detector OVERRIDE\n");
                ScoredBox* detectedBox = moreConfidentBoxList[0];
                // ImageBuilder* builder = new ImageBuilder(frame);
                // builder->withBox(detectedBox->box, Colors::BLUE)->withTitle("override")->display(0);
                Box* box = detectedBox->box;
                box->isValid = false;
                shouldLearn = false;
                maybeFinalBox = new Option<Box>(box);
            } else {
                // Detector.Combine
                printf("Tracker OK, Detector COMBINE\n");
                Box* combinedBox = combineClosestBoxes(scoredTrackBox, candidateBoxList);
                printf("COMBINED >>> %s\n", combinedBox->toCharArr());
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
    DEBUG(BOLD(WHITE("==== Integrate is completed ====\n")));

    // Evaluation
    printf(CYAN("==== Evaluate is started ====\n"));
    if (maybeFinalBox->isDefined() && maybeFinalBox->get()->isValid) {
        Box* finalBox = maybeFinalBox->get();
        bool evaluateResult = detector->evaluate(frame, finalBox, 489.4352, modelId);
        finalBox->isValid = evaluateResult;
        shouldLearn = evaluateResult;
    }
    printf(CYAN("==== Evaluate is completed ====\n"));

    // Learning
    printf(YELLOW("==== Learner is started ====\n"));
    if (shouldLearn) {
        Box* finalBox = maybeFinalBox->get();
        printf("Going to learn %s\n", finalBox->toCharArr());
        detector->learn(frame, finalBox, scoredBoxList, modelId);
        finalBox->isValid = true;
    } else {
        printf("Not going to learn\n");
    }

    DEBUGALL(
        detector->dumpDetector();
    )

    printf(YELLOW("==== Learner is completed ====\n"));

    return maybeFinalBox;
};

Option<ScoredBox>* TLD::validate2(Frame* current, Box* oldBox, Box* trackedBox, int modelId) {
    if (oldBox == nullptr) {
        printf("TLD >> No previous box\n");
        Option<ScoredBox>* none = new Option<ScoredBox>();
        return none;
    } else {
        if (trackedBox == nullptr) {
            printf("TLD >> No current box\n");
            Option<ScoredBox>* none = new Option<ScoredBox>();
            return none;
        } else {
            trackedBox->isValid = oldBox->isValid;
            println("TRACKEDBOX >>> %s", trackedBox->toCharArr());

            ScoredBox* validatedScoredBox = detector->validate(current, trackedBox, modelId);
            double validatedScore = validatedScoredBox->getScoreValue("nn", modelId);

            if (validatedScore > 0.7) {
                printf("TLD >> Valid box\n");
                trackedBox->isValid = true;
                Option<ScoredBox>* someTrackedBox = new Option<ScoredBox>(validatedScoredBox);
                return someTrackedBox;
            }

            printf("TLD >> Not validated box, %3.3f <= 0.7. Will trust previous one\n", validatedScore);
            Option<ScoredBox>* someTrackedBox = new Option<ScoredBox>(validatedScoredBox);
            return someTrackedBox;
        }
    }
}

TrackerResult* TLD::validate(Frame* current, Box* oldBox, Box* trackedBox, int modelId) {
    if (trackedBox == nullptr) {
        return new TrackerResult();
        // return new ScoredBox(trackedBox);
    } else {
        ScoredBox* validatedScoredBox = detector->validate(current, trackedBox, modelId);
        double validatedScore = validatedScoredBox->getScoreValue("nn", modelId);

        // ScoredBox* scoredBox = new ScoredBox(trackedBox);
        if (oldBox != nullptr) {
            validatedScoredBox->isDetected = false;
            validatedScoredBox->box->isValid = false;
        }

        validatedScoredBox->box->isValid = oldBox->isValid;

        if (validatedScore > 0.7) {
                validatedScoredBox->isDetected = true;
                validatedScoredBox->box->isValid = true;
        }

        if (current->id == 172) {
            validatedScoredBox->isDetected = true;
        }

        // NNScore* nnScore = new NNScore(NULL, relativeScores, conservativeScores);
        // scoredBox->withScore("nn", nnScore);

        double score = validatedScoredBox->getScoreValue("nn", 0);

        printf("VALIDATE >>> %s, %g\n", validatedScoredBox->box->toCharArr(), score);
        return new TrackerResult(validatedScoredBox);
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

    double x1 = trackBox->x1 * 10;
    double y1 = trackBox->y1 * 10;
    double x2 = trackBox->x2 * 10;
    double y2 = trackBox->y2 * 10;
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
    meanBox->isValid = trackBox->isValid;
    return meanBox;
}
