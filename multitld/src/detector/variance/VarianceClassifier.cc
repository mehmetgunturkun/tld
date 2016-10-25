#include "detector/variance/VarianceClassifier.hpp"

VarianceClassifier::VarianceClassifier() {
    this->initiated = false;
    this->minimumVariance = FLT_MAX;
}

VarianceClassifier::VarianceClassifier(double minVariance) {
    minimumVariance = minVariance;
}

void VarianceClassifier::init(Frame* frame, vector<Box*> boxList) {
    this->initiated = true;
    this->nrOfModels = (int) boxList.size();
    this->varianceList.resize(nrOfModels);
    this->minimumVariance = FLT_MAX;
}

void VarianceClassifier::setMinimumVariance(int modelId, double minVariance) {
    this->varianceList[modelId] = minVariance;
    if (this->minimumVariance > minVariance) {
        this->minimumVariance = minVariance;
    }
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
        if (box->variance >= variance) {
            classifiedModelIds.push_back(i);
        }
    }

    VarianceScore* score = new VarianceScore();
    score->classifiedModelIds = classifiedModelIds;
    score->isAnyModellClassified = box->variance >= minimumVariance;
    scoredBox->withScore("variance", score);

    return score->isAnyModellClassified;
}
