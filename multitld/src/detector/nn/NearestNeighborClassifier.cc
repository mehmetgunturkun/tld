#include "detector/nn/NearestNeighborClassifier.hpp"

NearestNeighborClassifier::NearestNeighborClassifier(Frame* firstFrame, vector<Box*> boxList) {
    nrOfModels = (int) boxList.size();
    for (int i = 0; i < nrOfModels; i++) {
        models.push_back(new ObjectModel());
    }

    POSITIVE_SCORE_THRESHOLD = 0.65;
}

bool NearestNeighborClassifier::classify(Frame* frame, ScoredBox* scoredBox) {
    Box* box = scoredBox->box;
    Patch* patch = new Patch(frame, box);
    vector<float> relativeScores(nrOfModels);
    vector<float> conservativeScores(nrOfModels);
    for (int i = 0;  i < nrOfModels; i++) {
        ObjectModel* objectModel = models[i];
        ObjectScore* objectScore = objectModel->computeScore(patch);

        relativeScores[i] = objectScore->relativeScore;
        conservativeScores[i] = objectScore->conservativeScore;
    }

    bool anyModelClassified = false;
    vector<int> classifiedModelIds;
    for (int j = 0; j < nrOfModels; j++) {
        float score = relativeScores[j];
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
    vector<float> relativeScores(nrOfModels);
    vector<float> conservativeScores(nrOfModels);

    ObjectModel* objectModel = models[modelId];
    ObjectScore* objectScore = objectModel->computeScore(patch);
    float conservativeScore = objectScore->conservativeScore;

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

        float relativeScore = objectScore->relativeScore;
        bool isInPositive = objectScore->isInPositive;

        // Frame* frame = ts.frame;
        // ImageBuilder* builder = new ImageBuilder(frame);
        if (label == 1) {
            // builder->withBox(scoredBox->box, Colors::BLUE)->withTitle("nn-pos")->display(0);
            if (relativeScore <= 0.65) {
                if (isInPositive == true) {
                    // int patchIndex = objectScore->getClosestPositivePatchIndex;
                    // printf("NN.rep(+) >>> %s\n", scoredBox->box->toCharArr());
                } else {
                    // printf("NN.add(+) >>> %s\n", scoredBox->box->toCharArr());
                    model->add(patch, true);
                }
            } else {
                // printf("NN.nop(+) >>> %s\n", scoredBox->box->toCharArr());
            }
        }

        if (label == 0) {
            // builder->withBox(scoredBox->box, Colors::RED)->withTitle("nn-neg")->display(0);
            if (relativeScore > 0.5) {
                // printf("NN.add(-) >>> %s\n", scoredBox->box->toCharArr());
                model->add(patch, false);
            } else {
                // printf("NN.nop(-) >>> %s\n", scoredBox->box->toCharArr());
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
