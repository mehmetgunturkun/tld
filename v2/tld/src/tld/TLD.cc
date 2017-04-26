#include "tld/TLD.hpp"

TLDResultSet::TLDResultSet(Box* maybeTrackedBox) {
    this->maybeTrackedBox = maybeTrackedBox;
}
void TLDResultSet::add(ScoredBox* scoredBox) {
    this->scoredBoxList.push_back(scoredBox);
}

TLD::TLD() {
    this->tracker = (StubbedTracker*) NULL;
    this->detector = new Detector();

    this->trackedBoxValidationScoreThreshold = 0.7;
    this->moreConfidentBoxOverlapThreshold = 0.5;
    this->minimumOverlapToCombine = 0.7;
}

TLD::TLD(StubbedTracker* tracker, Detector* detector) {
    this->tracker = tracker;
    this->detector = detector;
    this->nrOfModels = 0;
}

TLD::~TLD() {
    delete tracker;
    delete detector;
}

vector<Box*> TLD::init(Frame* frame, vector<Box*> boxList) {
    this->nrOfModels = (int) boxList.size();

    vector<Box*> correctedBox = detector->init(frame, boxList);
    printf("Initialization is completed\n");
    return correctedBox;
}

vector<Box*> TLD::track(Frame* prev, Frame* curr, vector<Box*> prevBoxList) {
    println("Frame(%s) >> Frame(%s)", prev->name.c_str(), curr->name.c_str());

    printf(GREEN("==== Tracker is started ====\n"));
    vector<Box*> currentBoxList = tracker->track(prev, curr, prevBoxList);
    printf(GREEN("==== Tracker is completed ====\n"));

    // printf(RED("==== Detector is started ====\n"));
    vector<ScoredBox*> scoredBoxList = detector->detect(curr);
    // printf(RED("==== Detector is completed ====\n"));

    vector<TLDResultSet*> resultSetPerModel = groupResults(currentBoxList, scoredBoxList);

    vector<Box*> estimatedBoxList;
    for (int modelId = 0; modelId < nrOfModels; modelId++) {
        TLDResultSet* resultSet = resultSetPerModel[modelId];
        Box* prevBox = prevBoxList[modelId];
        Option<Box*> maybeCurrentBox = integrate(curr, prevBox, resultSet->maybeTrackedBox, scoredBoxList, modelId);

        if (maybeCurrentBox.isDefined()) {
            Box* currentBox = maybeCurrentBox.get();
            printf("TLD >> ");
            currentBox->print();
            estimatedBoxList.push_back(currentBox);
        } else {
            estimatedBoxList.push_back(nullptr);
            printf("No valid result for %d!\n", modelId);
        }

        delete resultSet;
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
    ImageBuilder builder = ImageBuilder(frame);

    int nrOfBoxes = (int) b1.size();
    for (int i = 0; i < nrOfBoxes; i++) {
        ScoredBox* b = b1[i];
        if (b != nullptr) {
            builder.withBox(b->box, Colors::YELLOW);
        }
    }

    nrOfBoxes = (int) b2.size();
    for (int i = 0; i < nrOfBoxes; i++) {
        ScoredBox* b = b2[i];
        if (b != nullptr) {
            builder.withBox(b->box, Colors::RED);
        }
    }

    nrOfBoxes = (int) b3.size();
    for (int i = 0; i < nrOfBoxes; i++) {
        ScoredBox* b = b3[i];
        if (b != nullptr) {
            builder.withBox(b->box, Colors::BLUE);
        }
    }

    builder.withTitle("detection").display(1);
}

Option<Box*> TLD::integrate(Frame* frame, Box* oldBox, Box* maybeTrackedBox, vector<ScoredBox*> scoredBoxList, int modelId) {
    DEBUG(YELLOW("==== Validation is started ====\n"));
    Option<ScoredBox*> maybeScoredBox = validate(frame, oldBox, maybeTrackedBox, modelId);
    DEBUG(YELLOW("==== Validation is completed ====\n"));

    DEBUG(YELLOW("==== Partitioning is started ====\n"));
    DetectorResult* detectorResult = partition(scoredBoxList, modelId);
    vector<ScoredBox*> highVarianceBoxList = detectorResult->highVarianceBoxList;
    vector<ScoredBox*> candidateBoxList = detectorResult->candidateBoxList;
    vector<ScoredBox*> detectedBoxList = detectorResult->detectedBoxList;
    vector<ScoredBox*> clusteredBoxList = detectorResult->clusteredBoxList;
    DEBUG(YELLOW("==== Partitioning is completed ====\n"));


    bool shouldLearn = false;
    Option<Box*> maybeFinalBox;

    if (maybeScoredBox.isDefined()) {
        ScoredBox* scoredTrackBox = maybeScoredBox.get();
        shouldLearn = scoredTrackBox->box->isValid;

        if (detectedBoxList.size() > 0) {
            vector<ScoredBox*> moreConfidentBoxList = getMoreConfidentBoxList(scoredTrackBox, clusteredBoxList, modelId);
            if (moreConfidentBoxList.size() == 1) {
                printf("Tracker OK, Detector OVERRIDE\n");
                ScoredBox* detectedBox = moreConfidentBoxList[0];
                // ImageBuilder* builder = new ImageBuilder(frame);
                // builder->withBox(detectedBox->box, Colors::BLUE)->withTitle("override")->display(0);
                Box* box = detectedBox->box->clone();
                box->isValid = false;
                shouldLearn = false;
                maybeFinalBox = Option<Box*>(box);
            } else {
                // Detector.Combine
                printf("Tracker OK, Detector COMBINE\n");
                Box* combinedBox = combineClosestBoxes(scoredTrackBox, candidateBoxList);
                printf("COMBINED >>> ");
                combinedBox->print();
                maybeFinalBox = Option<Box*>(combinedBox);
                shouldLearn = shouldLearn && true;
            }
        } else {
            // Detector.Fail
            printf("Tracker OK, Detector FAILED\n");
            Box* trackedBox = scoredTrackBox->box->clone();
            maybeFinalBox = Option<Box*>(trackedBox);
        }

        delete scoredTrackBox;
    } else {
        // Tracker.Fail
        if (detectedBoxList.size() > 0) {
            if (clusteredBoxList.size() == 1) {
                printf("Tracker FAIL, Detector ONE-BOX\n");
                // Detector.Success
                ScoredBox* detectedBox = clusteredBoxList[0];
                maybeFinalBox = Option<Box*>(detectedBox->box->clone());
            } else if (clusteredBoxList.size() > 1) {
                // There are multiple boxes no way to decide!
                printf("Tracker FAIL, Detector MULTIPLE-BOX\n");
                maybeFinalBox = Option<Box*>();
            } else {
                // Detector.Fail
                printf("Tracker FAIL, Detector FAIL (SHOULDN'T HAVE HAPPENED)\n");
                maybeFinalBox = Option<Box*>();
            }
        } else {
            // Detector.Fail
            printf("Tracker FAIL, Detector FAIL\n");
            maybeFinalBox = Option<Box*>();
        }
    }

    // Evaluation
    // printf(CYAN("==== Evaluate is started ====\n"));
    if (maybeFinalBox.isDefined() && maybeFinalBox.get()->isValid) {
        Box* finalBox = maybeFinalBox.get();
        bool evaluateResult = detector->evaluate(frame, finalBox, modelId);
        finalBox->isValid = evaluateResult;
        shouldLearn = evaluateResult;
    }
    // printf(CYAN("==== Evaluate is completed ====\n"));

    // Learning
    // printf(YELLOW("==== Learner is started ====\n"));
    // shouldLearn = false;
    if (shouldLearn) {
        Box* finalBox = maybeFinalBox.get();
        printf("Going to learn %s\n", finalBox->toCharArr());
        detector->learn(frame, finalBox, scoredBoxList, modelId);
        finalBox->isValid = true;
    } else {
        printf("Not going to learn\n");
    }
    // printf(YELLOW("==== Learner is completed ====\n"));

    delete detectorResult;

    return maybeFinalBox;
};

Option<ScoredBox*> TLD::validate(Frame* current, Box* oldBox, Box* trackedBox, int modelId) {
    if (oldBox == nullptr) {
        printf("TLD >> No previous box\n");
        Option<ScoredBox*> none = Option<ScoredBox*>();
        return none;
    } else {
        if (trackedBox == nullptr) {
            printf("TLD >> No current box\n");
            Option<ScoredBox*> none = Option<ScoredBox*>();
            return none;
        } else {
            trackedBox->isValid = oldBox->isValid;
            println("TRACKEDBOX >>> %s", trackedBox->toCharArr());

            ScoredBox* validatedScoredBox = detector->validate(current, trackedBox, modelId);
            double validatedScore = validatedScoredBox->getScoreValue("nn", modelId);

            if (validatedScore > trackedBoxValidationScoreThreshold) {
                printf("TLD >> Valid box\n");
                trackedBox->isValid = true;
                Option<ScoredBox*> someTrackedBox = Option<ScoredBox*>(validatedScoredBox);
                return someTrackedBox;
            }

            printf("TLD >> Not validated box, %3.3f <= 0.7. Will trust previous one\n", validatedScore);
            Option<ScoredBox*> someTrackedBox = Option<ScoredBox*>(validatedScoredBox);
            return someTrackedBox;
        }
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
    printf(YELLOW("==== Clustering is started ====\n"));
    vector<ScoredBox*> clusteredBoxList = ScoredBox::cluster(detectedBoxList, (int) detectedBoxList.size());
    printf(YELLOW("==== Clustering is completed ====\n"));

    for (int i = 0; i < (int) clusteredBoxList.size(); i++) {
        printf("CB >> %s, %f\n", clusteredBoxList[i]->box->toCharArr(), clusteredBoxList[i]->getScoreValue("nn", 0));
    }

    return new DetectorResult(scoredBoxList, candidateBoxList, detectedBoxList, clusteredBoxList, modelId);
}

vector<ScoredBox*> TLD::getMoreConfidentBoxList(ScoredBox* trackScoredBox, vector<ScoredBox*> detectedBoxes, int modelId) {
    vector<ScoredBox*> moreConfidentBoxList;
    int nrOfBoxes = (int) detectedBoxes.size();

    float trackScore = trackScoredBox->getScoreValue("nn", modelId);
    Box* trackBox = trackScoredBox->box;

    for (int i = 0; i < nrOfBoxes; i++) {
        ScoredBox* scoredBox = detectedBoxes[i];
        float detectScore = scoredBox->getScoreValue("nn", modelId);

        float overlap = Box::computeOverlap(trackBox, scoredBox->box);
        if (overlap < moreConfidentBoxOverlapThreshold && detectScore > trackScore) {
            moreConfidentBoxList.push_back(scoredBox);
        }
    }

    return moreConfidentBoxList;
}

Box* TLD::combineClosestBoxes(ScoredBox* trackScoredBox, vector<ScoredBox*> detectedBoxes) {
    Box* trackBox = trackScoredBox->box;

    double x1 = trackBox->x1 * 10.0;
    double y1 = trackBox->y1 * 10.0;
    double x2 = trackBox->x2 * 10.0;
    double y2 = trackBox->y2 * 10.0;
    int patchCount = 10;

    int patchSize = (int) detectedBoxes.size();
    for (int i = 0; i < patchSize; i++) {
        ScoredBox* scoredBox = detectedBoxes[i];
        Box* box = scoredBox->box;
        float overlap = Box::computeOverlap(trackBox, box);
        if (overlap > minimumOverlapToCombine) {
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
