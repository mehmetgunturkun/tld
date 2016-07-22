#include "detector/nn/ObjectModel.hpp"

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

double ObjectModel::computeInitialPositiveSimilarity(Patch* sample) {
    return computeNearestSimilarity(positivePatchList, ceil(nrOfPositivePatches / 2), sample);
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
