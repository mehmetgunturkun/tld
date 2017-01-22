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

double computePatchVariance(Patch* p) {
    Mat* patchData = p->data;
    double sum = 0.0;
    for (int i = 0 ; i < Patch::HEIGHT; i++) {
        for (int j = 0 ; j < Patch::WIDTH; j++) {
            double p = patchData->at<double>(i, j);
            sum += p;
        }
    }

    double mean = sum / 225;

    double variance = 0.0;
    for (int i = 0 ; i < Patch::HEIGHT; i++) {
        for (int j = 0 ; j < Patch::WIDTH; j++) {
            double p = patchData->at<double>(i, j);
            p = p - mean;
            variance += (p * p);
        }
    }

    variance = variance / 224;
    return variance;
}

double NearestNeighborClassifier::getPatchVariance(Frame* frame, Box* box) {
    Patch* patch = new Patch(frame, box);
    double variance = computePatchVariance(patch);
    return variance / 2.0;
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
    printf("#P = %d, #N = %d\n", model->nrOfPositivePatches, model->nrOfNegativePatches);
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
        return patchList;
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

        return samples;
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

        if (label == 1 && relativeScore <= 0.65) {
            if (isInPositive == true) {
                model->add(patch, true);
                continue;
            }
            model->add(patch, closestPositivePatchIndex, true);
        }

        if (label == 0 && relativeScore > 0.5) {
            model->add(patch, false);
        }
    }
}
