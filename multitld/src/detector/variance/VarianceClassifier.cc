#include "detector/variance/VarianceClassifier.hpp"

VarianceClassifier::VarianceClassifier(Frame* frame, vector<Box*> boxList) {
    this->minimumVariance = INT_MAX;
    this->nrOfModels = (int) boxList.size();
    this->varianceList.resize(nrOfModels);

    for (int i = 0; i < this->nrOfModels; i++) {
            Box* b = boxList[i];
            MeanVariance* initialMeanVariance = frame->integral->computeMeanVariance(
                (int) b->x1,
                (int) b->y1,
                (int) b->width,
                (int) b->height
            );

            this->varianceList[i] = initialMeanVariance->variance * 0.5;
            if (minimumVariance > initialMeanVariance->variance) {
                minimumVariance = initialMeanVariance->variance;
            }
    }
    minimumVariance = minimumVariance * 0.5;
}

VarianceClassifier::VarianceClassifier(double minVariance) {
    minimumVariance = minVariance;
}

bool VarianceClassifier::classify(Frame *frame, ScoredBox *scoredBox) {
    Box* box = scoredBox->box;
    MeanVariance* meanVariance = frame->integral->computeMeanVariance(
        (int) box->x1,
        (int) box->y1,
        (int) box->width,
        (int) box->height);

    box->mean = meanVariance->mean;
    box->variance = meanVariance->variance;

    vector<int> classifiedModelIds;
    for (int i = 0; i < this->nrOfModels; i++) {
        float variance = this->varianceList[i];
        if (box->variance > variance) {
            classifiedModelIds.push_back(i);
        }
    }

    VarianceScore* score = new VarianceScore();
    score->classifiedModelIds = classifiedModelIds;
    score->isAnyModellClassified = box->variance > minimumVariance;
    scoredBox->withScore("variance", score);

    return score->isAnyModellClassified;
}
