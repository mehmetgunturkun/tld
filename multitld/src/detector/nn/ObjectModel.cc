#include "detector/nn/ObjectModel.hpp"

ObjectScore::ObjectScore(double relativeScore, double conservativeScore, bool isInPositive, bool isInNegative) {
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
    if (nrOfPositivePatches == 0) {
        ObjectScore* objectScore = new ObjectScore(0.0, 0.0, false, false);
        return objectScore;
    }

    if (nrOfNegativePatches == 0) {
        ObjectScore* objectScore = new ObjectScore(1.0, 1.0, false, false);
        return objectScore;
    }

    double positiveNNSimilarity = computePositiveSimilarity(sample);
    double positiveInititalNNSimilarity = computeInitialPositiveSimilarity(sample);
    double negativeNNSimilarity = computeNegativeSimilarity(sample);

    double relativeScore = 0.0;
    if ( positiveNNSimilarity != 0.0 || negativeNNSimilarity != 0.0) {
        relativeScore  = (negativeNNSimilarity / (positiveNNSimilarity + negativeNNSimilarity));
    }

    double conservativeScore = 0.0;
    if ( positiveInititalNNSimilarity != 0.0 || negativeNNSimilarity != 0.0) {
        conservativeScore  = (negativeNNSimilarity / (positiveInititalNNSimilarity + negativeNNSimilarity));
    }

    bool isInPositive = (1.0 - positiveNNSimilarity) > 0.95;
    bool isInNegative = (1.0 - negativeNNSimilarity) > 0.95;

    ObjectScore* objectScore = new ObjectScore(relativeScore, conservativeScore, isInPositive, isInNegative);
    return objectScore;
}

double ObjectModel::computeInitialPositiveSimilarity(Patch* sample) {
    return computeNearestSimilarity(positivePatchList, ceil( (double) nrOfPositivePatches / 2), sample);
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
            double tempSimilarityValue = computeSimilarity(templatePatch, sample);
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
