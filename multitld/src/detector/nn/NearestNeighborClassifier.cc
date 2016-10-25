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

void NearestNeighborClassifier::train(TrainingSet<ScoredBox> ts, int modelId) {
    vector<Labelled<ScoredBox>*> samples = ts.getPositiveFirstSamples();
    ObjectModel* model = models[modelId];
    int nrOfSamples = (int) samples.size();
    for (int i = 0; i < nrOfSamples; i++) {
        Labelled<ScoredBox>* sample = samples[i];

        ScoredBox* scoredBox = sample->item;
        int label = sample->label;

        NNScore* nnScore = (NNScore*) scoredBox->getScore("nn");
        Patch* patch = nnScore->patch;
        ObjectScore* objectScore = model->computeScore(patch);

        double relativeScore = objectScore->relativeScore;
        bool isInPositive = objectScore->isInPositive;
        int closestPositivePatchIndex = objectScore->closestPositivePatchIndex;

        if (label == 1) {
            if (relativeScore <= 0.65) {
                if (isInPositive == true) {
                    printf("NN.add(+) >>> %d, %s\n", closestPositivePatchIndex, scoredBox->box->toCharArr());
                    model->add(patch, true);
                    continue;
                }

                printf("NN.ins(+) >>> %s, %g, %d, %d\n", scoredBox->box->toCharArr(), relativeScore, isInPositive, closestPositivePatchIndex);
                model->add(patch, closestPositivePatchIndex, true);
            } else {
                printf("NN.nop(+) >>> %s, %g, %d\n", scoredBox->box->toCharArr(), relativeScore, isInPositive);
            }
        }

        if (label == 0) {
            // builder->withBox(scoredBox->box, Colors::RED)->withTitle("nn-neg")->display(0);
            if (relativeScore > 0.5) {
                printf("NN.add(-) >>> %s, %g\n", scoredBox->box->toCharArr(), relativeScore);
                model->add(patch, false);
            } else {
                printf("NN.nop(-) >>> %s, %g\n", scoredBox->box->toCharArr(), relativeScore);
            }
        }
    }
}

bool NearestNeighborClassifier::evaluate(Frame* frame, Box* box, int modelId) {
    Patch* patch = new Patch(frame, box);
    ObjectModel* model = models[modelId];
    ObjectScore* objectScore = model->computeScore(patch);
    return objectScore->relativeScore > 0.7;
}

void NearestNeighborClassifier::dumpNearestNeighborClassifier() {
    ObjectModel* model = models[0];
    printf("#P = %d, #N = %d\n", model->nrOfPositivePatches, model->nrOfNegativePatches);
}
