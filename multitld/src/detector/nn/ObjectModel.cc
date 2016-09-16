#include "detector/nn/ObjectModel.hpp"

ObjectScore::ObjectScore(float relativeScore, float conservativeScore, bool isInPositive, bool isInNegative) {
    this->relativeScore = relativeScore;
    this->conservativeScore = conservativeScore;
    this->isInPositive = isInPositive;
    this->isInNegative = isInNegative;
}

ObjectModel::ObjectModel() {
    nrOfPositivePatches = 0;
    nrOfNegativePatches = 0;
}

double ObjectModel::computeRelativeScore(Patch* sample) {
    double positiveNNSimilarity = computePositiveSimilarity(sample);
    double negativeNNSimilarity = computeNegativeSimilarity(sample);
    if ( positiveNNSimilarity == 0 && negativeNNSimilarity == 0) {
        return 0.0;
    } else {
        double sim  = (negativeNNSimilarity / (positiveNNSimilarity + negativeNNSimilarity));
        return sim;
    }
}

double ObjectModel::computeConservativeScore(Patch* sample) {
    double positiveNNSimilarity = computeInitialPositiveSimilarity(sample);
    double negativeNNSimilarity = computeNegativeSimilarity(sample);

    if ( positiveNNSimilarity == 0 && negativeNNSimilarity == 0) {
        return 0.0;
    } else {
        double sim  = (negativeNNSimilarity / (positiveNNSimilarity + negativeNNSimilarity));
        return sim;
    }
}

ObjectScore* ObjectModel::computeScore(Patch* sample) {
    if (nrOfPositivePatches == 0 && nrOfNegativePatches == 0) {
        ObjectScore* objectScore = new ObjectScore(0.0f, 0.0f, false, false);
        return objectScore;
    }

    float positiveNNSimilarity = (float) computePositiveSimilarity(sample);
    float positiveInititalNNSimilarity = (float) computeInitialPositiveSimilarity(sample);
    float negativeNNSimilarity = (float) computeNegativeSimilarity(sample);

    float relativeScore = 0.0;
    if ( positiveNNSimilarity != 0.0f || negativeNNSimilarity != 0.0f) {
        relativeScore  = (negativeNNSimilarity / (positiveNNSimilarity + negativeNNSimilarity));
    }

    float conservativeScore = 0.0;
    if ( positiveInititalNNSimilarity != 0.0f || negativeNNSimilarity != 0.0f) {
        conservativeScore  = (negativeNNSimilarity / (positiveInititalNNSimilarity + negativeNNSimilarity));
    }

    bool isInPositive= positiveNNSimilarity > 0.95;
    bool isInNegative = negativeNNSimilarity > 0.95;

    ObjectScore* objectScore = new ObjectScore(relativeScore, conservativeScore, isInPositive, isInNegative);
    return objectScore;
}

double ObjectModel::computeInitialPositiveSimilarity(Patch* sample) {
    return computeNearestSimilarity(positivePatchList, ceil( (float) nrOfPositivePatches / 2), sample);
}

double ObjectModel::computePositiveSimilarity(Patch* sample) {
    return computeNearestSimilarity(positivePatchList, nrOfPositivePatches, sample);
}

double ObjectModel::computeNegativeSimilarity(Patch* sample) {
    return computeNearestSimilarity(negativePatchList, nrOfNegativePatches, sample);
}

double ObjectModel::computeNearestSimilarity(vector<Patch*> templatePatchList, int size, Patch* sample) {
    double similarityValue = 0.0;
    for (int i = 0; i < size; i++) {
            Patch* templatePatch = templatePatchList[i];
            float tempSimilarityValue = computeSimilarity(templatePatch, sample);
            if (tempSimilarityValue > similarityValue) {
                    similarityValue = tempSimilarityValue;
            }
    }
    return (1.0 - similarityValue);
}

double ObjectModel::computeSimilarity(Patch* templatePatch, Patch* samplePatch) {
    Mat* templateImg = templatePatch->data;
    Mat* sampleImg = samplePatch->data;

    double corr = 0.0;
    double normTemplate = 0.0;
    double normSample = 0.0;

    for (int i = 0; i < Patch::HEIGHT; i++) {
        for (int j = 0; j < Patch::WIDTH; j++) {
            double templateVariance = templateImg->at<double>(i,j);
            double sampleVariance = sampleImg->at<double>(i,j);

            corr += templateVariance*sampleVariance;
            normTemplate += templateVariance*templateVariance;
            normSample += sampleVariance*sampleVariance;
        }
    }

    double upper = corr;
    double lower = sqrt(normTemplate * normSample);
    double ncc = (upper / lower);
    return 0.5 * (ncc + 1.0);
}

void ObjectModel::add(Patch* patch, bool label) {
    if (label) {
        positivePatchList.push_back(patch);
        nrOfPositivePatches += 1;
    } else {
        negativePatchList.push_back(patch);
        nrOfNegativePatches += 1;
    }
}

void ObjectModel::replace(Patch* patch, int oldIndex) {
    // this->add(patch, true);
}
