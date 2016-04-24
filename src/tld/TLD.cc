#include "tld/TLD.hpp"

TLD::TLD(Tracker* t, CascadedSingleDetector* d) {
    tracker = t;
    detector = d;
}

TLD::TLD(Frame* f, Box* b) {
    minOverlap = Conf::getDouble("tld.minOverlap", 0.8);
    minValidationScore = Conf::getDouble("tld.minValidationScore", 0.8);

    tracker = new Tracker();
    detector = new CascadedSingleDetector(f, b);

    init(f, b);
}

void TLD::init(Frame* firstFrame, Box* firstBox) {
    detector->init(firstFrame, firstBox);
}

Option<Box>* TLD::process(Frame *prev, Frame *current, Option<Box> *maybePrevBox) {
    TrackResult* trackResult = track(prev, current, maybePrevBox);
    DetectResult* detectResult = detect(current);
    Option<Box>* currentBox = integrate(current, trackResult, detectResult);
    return currentBox;
}

TrackResult* TLD::track(Frame* prev, Frame* current, Option<Box>* maybePrevBox) {
    if (maybePrevBox->isEmpty()) {
        return TrackResult::empty;
    } else {
        Box* prevBox = maybePrevBox->get();
        Option<Box>* maybeBox = tracker->track(prev, current, prevBox);
        TrackResult* validatedTrackResult = validate(current, maybeBox);
        return validatedTrackResult;
    }
}

TrackResult* TLD::validate(Frame* current, Option<Box>* maybeBox) {
    if (maybeBox->isEmpty()) {
        return TrackResult::empty;
    } else {
        Box* box = maybeBox->get();
        ClassificationDetails* detail = detector->score(current, box);
        if (detail->score > minValidationScore) {
            //TODO Define appropriate key instead of UNDEFINED
            ScoredBox* scoredBox = new ScoredBox(box);
            scoredBox->withScore("ensemble", detail);
            TrackResult* trackResult = new TrackResult(scoredBox);
            return trackResult;
        } else {
            TrackResult* failedTrackResult = TrackResult::empty;
            return failedTrackResult;
        }
    }
}

DetectResult* TLD::detect(Frame* current) {
    DetectResult* detectResult = detector->detect(current);
    return detectResult;
}

Option<Box>* TLD::integrate(Frame* current, TrackResult* trackResult, DetectResult* detectResult) {
    if (trackResult->isFailed) {
        //Tracker is failed.
        //Only way to move on is that
        //detector must classify ONE AND ONLY ONE box.
        if (detectResult->isFailed) {
            //Detector failed to classify any box as an object instance
            Option<Box>* none = new Option<Box>();
            return none;
        } else {
            //Detector classify at least one box as an object instance
            if (detectResult->detectedSize == 1) {
                //There is only one unique detected box
                vector<ScoredBox*> detectedBoxes = detectResult->detected;
                ScoredBox* detectedBox = detectedBoxes[0];
                Option<Box>* maybeDetectedBox = new Option<Box>(detectedBox->box);
                return maybeDetectedBox;
            } else if (detectResult->detectedSize > 1) {
                //There are multiple unique boxes detected
                //and cannot decide which one is correct one!
                Option<Box>* none = new Option<Box>();
                return none;
            } else {
                //Impossible case just to satisfy compiler!
                Option<Box>* none = new Option<Box>();
                return none;
            }
        }
    } else {
        //Tracker is successful
        if (detectResult->isFailed) {
            //Detector failed to classify any box as an object instance
            //This is why we should learn this box.
            ScoredBox* trackedScoredBox = trackResult->getBox();
            Box* trackedBox = trackedScoredBox->box;
            learn(current, trackedBox, detectResult);
            Option<Box>* maybeTrackedBox = new Option<Box>(trackedBox);
            return maybeTrackedBox;
        } else {
            //Detector classify at least one box as an object instance
            if (isThereMoreConfidentOneBox(trackResult, detectResult)) {
                //If there is an only one box has more confidence value than trackedBox
                //Then fix tracker trajectory.
                vector<ScoredBox*> detectedBoxes = detectResult->detected;
                ScoredBox* mostConfidentBox = detectedBoxes[0];
                Option<Box>* maybeDetectedBox = new Option<Box>(mostConfidentBox->box);
                return maybeDetectedBox;
            } else {
                // Otherwise combine closest detected boxes with tracked box
                // and ignore less confident and further boxes
                Box* combined = combineClosestBoxes(trackResult, detectResult);
                learn(current, combined, detectResult);
                Option<Box>* combinedBox = new Option<Box>(combined);
                return combinedBox;
            }
        }
    }
}

bool TLD::isThereMoreConfidentOneBox(TrackResult* trackResult, DetectResult* detectResult) {
    int detectedSize = detectResult->detectedSize;
    if (detectedSize == 1) {
        bool moreConfident = false;

        ScoredBox* trackScoredBox = trackResult->getBox();
        double trackScore = trackScoredBox->getScore("nn");
        Box* trackBox = trackScoredBox->box;

        vector<ScoredBox*> detectedBoxes = detectResult->detected;
        for (int i = 0; i < detectedSize; i++) {
            ScoredBox* scoredBox = detectedBoxes[i];
            double detectScore = scoredBox->getScore("nn");
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

Box* TLD::combineClosestBoxes(TrackResult* trackResult, DetectResult* detectResult) {
    int patchSize = detectResult->detectedSize;
    vector<ScoredBox*> detectedBoxes = detectResult->detected;

    ScoredBox* trackScoredBox = trackResult->getBox();
    Box* trackBox = trackScoredBox->box;

    float x1 = trackBox->x1*10;
    float y1 = trackBox->y1*10;
    float x2 = trackBox->x2*10;
    float y2 = trackBox->y2*10;
    int patchCount = 10;

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

void TLD::learn(Frame* current, Box* trackedBox, DetectResult* detectResult) {
    // TODO Out of order for now!
    // detector->learn(current, trackedBox, detectResult);
}
