#include "detector/nn/NearestNeighborClassifier.hpp"

NearestNeighborClassifier::NearestNeighborClassifier() {
    POSITIVE_SCORE_THRESHOLD = 0.65;
}

void NearestNeighborClassifier::init(Frame* firstFrame, vector<Box*> boxList) {
    this->nrOfModels = (int) boxList.size();
    for (int i = 0; i < nrOfModels; i++) {
        this->models.push_back(new ObjectModel());
    }
}

NearestNeighborClassifier::~NearestNeighborClassifier() {
    for (int i = 0; i < nrOfModels; i++) {
        ObjectModel* model = this->models[i];
        delete model;
    }
}

double computePatchVariance(Patch* p) {
    return Patch::computeVariance(p);
}

double NearestNeighborClassifier::getPatchVariance(Frame* frame, Box* box) {
    Patch* patch = new Patch(frame, box);
    double variance = Patch::computeVariance(patch);

    delete patch;
    return variance;
}

bool NearestNeighborClassifier::classify(Frame* frame, ScoredBox* scoredBox) {
    Box* box = scoredBox->box;
    Patch* patch = new Patch(frame, box);

    vector<double> relativeScores(nrOfModels);
    vector<double> conservativeScores(nrOfModels);

    for (int i = 0;  i < nrOfModels; i++) {
        ObjectModel* objectModel = models[i];
        ObjectScore* objectScore = objectModel->computeScore(patch);

        relativeScores[i] = objectScore->relativeScore;
        conservativeScores[i] = objectScore->conservativeScore;

        delete objectScore;
    }

    bool anyModelClassified = false;
    vector<int> classifiedModelIds;
    for (int j = 0; j < nrOfModels; j++) {
        double score = relativeScores[j];
        if (score > POSITIVE_SCORE_THRESHOLD) {
            anyModelClassified = true;
            classifiedModelIds.push_back(j);
        }
    }

    NNScore* score = new NNScore(patch, relativeScores, conservativeScores);
    score->isAnyModellClassified = anyModelClassified;
    score->classifiedModelIds = classifiedModelIds;
    scoredBox->withScore("nn", score);

    return score->isAnyModellClassified;
}

bool NearestNeighborClassifier::validate(Frame* frame, ScoredBox* scoredBox, int modelId) {
    Box* box = scoredBox->box;
    Patch* patch = new Patch(frame, box);
    vector<double> relativeScores(nrOfModels);
    vector<double> conservativeScores(nrOfModels);

    ObjectModel* objectModel = models[modelId];
    ObjectScore* objectScore = objectModel->computeScore(patch);
    double conservativeScore = objectScore->conservativeScore;

    relativeScores[modelId] = objectScore->relativeScore;
    conservativeScores[modelId] = conservativeScore;

    bool anyModelClassified = false;
    vector<int> classifiedModelIds;
    if (conservativeScore > 0.7) {
        anyModelClassified = true;
        // printf("High conservative score - %f!\n", conservativeScore);
        classifiedModelIds.push_back(modelId);
    } else {
        // printf("Low conservative score - %f!\n", conservativeScore);
    }

    NNScore* score = new NNScore(patch, relativeScores, conservativeScores);
    score->isAnyModellClassified = anyModelClassified;

    score->classifiedModelIds = classifiedModelIds;
    scoredBox->withScore("nn", score);
    scoredBox->isDetected = anyModelClassified;

    return score->isAnyModellClassified;
}

void NearestNeighborClassifier::score(Frame* frame, ScoredBox* scoredBox) {
    if (scoredBox->isScored("nn")) {
        // Already got a nn score
    } else {
        Box* box = scoredBox->box;
        Patch* patch = new Patch(frame, box);
        NNScore* nnScore = new NNScore(patch);

        scoredBox->withScore("nn", nnScore);
    }
}

bool NearestNeighborClassifier::evaluate(Frame* frame, Box* box, double minVariance, int modelId) {
    Patch* patch = new Patch(frame, box);
    ObjectModel* model = models[modelId];
    ObjectScore* objectScore = model->computeScore(patch);

    printf("EVALUATE >> %s, %3.5f\n", box->toCharArr(), objectScore->relativeScore);

    if (objectScore->relativeScore < 0.5) {
        printf("NN >> Fast Change\n");
        return false;
    }

    double patchVariance = computePatchVariance(patch);
    if (patchVariance < minVariance) {
        printf("NN >> Low Variance\n");
        return false;
    }

    return true;
}

void NearestNeighborClassifier::dumpNearestNeighborClassifier() {
    ObjectModel* model = models[0];
    int nrOfPositivePatches = model->nrOfPositivePatches;
    printf("#P = %d\n", nrOfPositivePatches);
    for (int i = 0; i < nrOfPositivePatches; i++) {
        printf("%d, ", model->positivePatchList[i]->id);
    }
    printf("\n");

    int nrOfNegativePatches = model->nrOfNegativePatches;
    printf("#N = %d\n", nrOfNegativePatches);
    for (int i = 0; i < nrOfNegativePatches; i++) {
        printf("%d, ", model->negativePatchList[i]->id);
    }
    printf("\n");
}

vector<Labelled<Patch>*> NearestNeighborClassifier::generateSamples(
    Frame* frame,
    vector<Box*> positiveBoxList,
    vector<Box*> negativeBoxList) {
        vector<Labelled<Patch>*> patchList;

        int nrOfPositiveSamples = (int) positiveBoxList.size();
        for (int i = 0; i < nrOfPositiveSamples; i++) {
            Box* box = positiveBoxList[i];
            Patch* patch = new Patch(frame, box);
            patchList.push_back(new Labelled<Patch>(patch, 1));
        }

        int nrOfNegativeSamples = (int) negativeBoxList.size();
        for (int i = 0; i < nrOfNegativeSamples; i++) {
            Box* box = negativeBoxList[i];
            Patch* patch = new Patch(frame, box);
            patchList.push_back(new Labelled<Patch>(patch, 0));
        }

        Random::seed();
        vector<Labelled<Patch>*> shuffledPatchList = Random::randomSample(patchList);

        if (nrOfPositiveSamples > 0) {
            Box* box = positiveBoxList[0];
            Patch* patch = new Patch(frame, box);

            shuffledPatchList.insert(shuffledPatchList.begin(), new Labelled<Patch>(patch, 1));
        }

        return shuffledPatchList;
}

vector<Labelled<Patch>*> NearestNeighborClassifier::generateSamples(
    Frame* frame,
    vector<ScoredBox*> positiveBoxList,
    vector<ScoredBox*> negativeBoxList) {
        vector<Labelled<Patch>*> samples;

        int nrOfPositiveSamples = (int) positiveBoxList.size();
        for (int i = 0; i < nrOfPositiveSamples; i++) {
            ScoredBox* scoredBox = positiveBoxList[i];
            Box* box = scoredBox->box;
            NNScore* nnScore = (NNScore*) scoredBox->getScore("nn");
            Patch* patch;
            if (nnScore == NULL) {
                patch = nnScore->patch;
            } else {
                patch = new Patch(frame, box);
            }

            samples.push_back(new Labelled<Patch>(patch, 1));
        }

        int nrOfNegativeSamples = (int) negativeBoxList.size();
        for (int i = 0; i < nrOfNegativeSamples; i++) {
            ScoredBox* scoredBox = negativeBoxList[i];
            Box* box = scoredBox->box;

            NNScore* nnScore = (NNScore*) scoredBox->getScore("nn");
            Patch* patch;
            if (nnScore == NULL) {
                patch = nnScore->patch;
            } else {
                patch = new Patch(frame, box);
            }
            samples.push_back(new Labelled<Patch>(patch, 0));
        }

        Random::seed();
        vector<Labelled<Patch>*> shuffledSamples = Random::randomSample(samples);

        if (nrOfPositiveSamples > 0) {
            ScoredBox* scoredBox = positiveBoxList[0];
            Box* box = scoredBox->box;
            NNScore* nnScore = (NNScore*) scoredBox->getScore("nn");
            Patch* patch;
            if (nnScore == NULL) {
                patch = nnScore->patch;
            } else {
                patch = new Patch(frame, box);
            }

            shuffledSamples.insert(shuffledSamples.begin(), new Labelled<Patch>(patch, 1));
        }

        return shuffledSamples;
}

void NearestNeighborClassifier::train(TrainingSet<Box> ts, int modelId) {
    Frame* frame = ts.frame;
    vector<Box*> positiveBoxList = ts.positiveSamples;
    vector<Box*> negativeBoxList = ts.negativeSamples;

    vector<Labelled<Patch>*> samples = generateSamples(
        frame,
        positiveBoxList,
        negativeBoxList
    );
    doTrain(samples, modelId);
}

void NearestNeighborClassifier::train(TrainingSet<ScoredBox> ts, int modelId) {
    Frame* frame = ts.frame;
    vector<ScoredBox*> positiveBoxList = ts.positiveSamples;
    vector<ScoredBox*> negativeBoxList = ts.negativeSamples;

    vector<Labelled<Patch>*> samples = generateSamples(
        frame,
        positiveBoxList,
        negativeBoxList
    );
    doTrain(samples, modelId);
}

void NearestNeighborClassifier::doTrain(vector<Labelled<Patch>*> samples, int modelId) {
    ObjectModel* model = models[modelId];

    int nrOfSamples = (int) samples.size();
    for (int i = 0; i < nrOfSamples; i++) {
        Labelled<Patch>* labelledPatch = samples[i];
        Patch* patch = labelledPatch->item;
        int label = labelledPatch->label;

        ObjectScore* objectScore = model->computeScore(patch);
        double relativeScore = objectScore->relativeScore;
        bool isInPositive = objectScore->isInPositive;
        int closestPositivePatchIndex = objectScore->closestPositivePatchIndex;

        if (label == 1) {
            if (relativeScore <= 0.65) {
                // printf("%4d. NNC(+) >>> Model(%5d).update\n", i + 1, patch->id);
                labelledPatch->added = true;
                if (isInPositive == true) {
                    model->add(patch, true);
                    continue;
                }

                model->add(patch, closestPositivePatchIndex, true);
            } else {
                // printf("%4d. NNC(+) >>> Model(%5d).ignore. Relative Score: %4.3f\n", i + 1, patch->id, relativeScore);
            }
        }

        if (label == 0) {
            if (relativeScore > 0.5) {
                // printf("%4d. NNC(-) >>> Model(%5d).update\n", i + 1, patch->id);
                labelledPatch->added = true;
                model->add(patch, false);
            } else {
                // printf("%4d. NNC(-) >>> Model(%5d).ignore. Relative Score: %4.3f\n", i + 1, patch->id, relativeScore);
            }
        }

        delete objectScore;
    }

    for (int i = 0; i < nrOfSamples; i++) {
        Labelled<Patch>* labelledPatch = samples[i];
        if (!(labelledPatch->added)) {
            delete labelledPatch->item;
        }
        delete labelledPatch;
    }
}
