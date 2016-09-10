#include "detector/nn/NearestNeighborClassifier.hpp"

NearestNeighborClassifier::NearestNeighborClassifier(Frame* firstFrame, vector<Box*> boxList) {
    nrOfModels = (int) boxList.size();
    for (int i = 0; i < nrOfModels; i++) {
        models.push_back(new ObjectModel());
    }

    POSITIVE_SCORE_THRESHOLD = 0.6;
}

bool NearestNeighborClassifier::classify(Frame* frame, ScoredBox* scoredBox) {
    Box* box = scoredBox->box;
    Patch* patch = new Patch(frame, box);
    vector<float> relativeScores(nrOfModels);
    vector<float> conservativeScores(nrOfModels);
    for (int i = 0;  i < nrOfModels; i++) {
        ObjectModel* objectModel = models[i];
        float relativeScore = objectModel->computeRelativeScore(patch);
        float conservativeScore = objectModel->computeConservativeScore(patch);

        relativeScores[i] = relativeScore;
        conservativeScores[i] = conservativeScore;
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
    float relativeScore = objectModel->computeRelativeScore(patch);
    float conservativeScore = objectModel->computeConservativeScore(patch);

    relativeScores[modelId] = relativeScore;
    conservativeScores[modelId] = conservativeScore;

    bool anyModelClassified = false;
    vector<int> classifiedModelIds;
    if (conservativeScore > 0.5) {
        anyModelClassified = true;
        classifiedModelIds.push_back(modelId);
    }

    NNScore* score = new NNScore(patch, relativeScores, conservativeScores);
    score->isAnyModellClassified = anyModelClassified;

    score->classifiedModelIds = classifiedModelIds;
    scoredBox->withScore("nn", score);
    scoredBox->isDetected = anyModelClassified;

    return score->isAnyModellClassified;
}

void NearestNeighborClassifier::train(TrainingSet<Box> ts, int modelId) {
    vector<Labelled<Box>> samples = ts.getLabelledSamples(false);
    printf("NN >> %lu samples are going to be processed for training\n", samples.size());
    ObjectModel* model = models[modelId];
    for (int i = 0; i < ts.nrOfSamples; i++) {
        Labelled<Box> sample = samples[i];
        Frame* frame = sample.frame;
        Box* box = sample.item;
        int label = sample.label;
        Patch* patch = new Patch(frame, box);
        model->add(patch, label == 1);
    }
    printf("NN >> %lu samples were processed for training\n", samples.size());
}

void NearestNeighborClassifier::train(TrainingSet<ScoredBox> ts, int modelId) {
    vector<Labelled<ScoredBox>> samples = ts.getLabelledSamples(false);
    printf("NN >> %lu samples are going to be processed for training\n", samples.size());
    ObjectModel* model = models[modelId];
    for (int i = 0; i < ts.nrOfSamples; i++) {
        Labelled<ScoredBox> sample = samples[i];
        Frame* frame = sample.frame;
        ScoredBox* scoredBox = sample.item;
        int label = sample.label;

        NNScore* nnScore = (NNScore*) scoredBox->getScore("nn");
        if (nnScore == NULL) {
            classify(frame, scoredBox);
            nnScore = (NNScore*) scoredBox->getScore("nn");
        }
        Patch* patch = nnScore->patch;
        model->add(patch, label == 1);
    }
    printf("NN >> %lu samples were processed for training\n", samples.size());
}
