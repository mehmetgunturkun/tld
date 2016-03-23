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
    //TODO NotImplemented
    return 0.0;
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

    float corr = 0.0;
    float normTemplate = 0.0;
    float normSample = 0.0;

    for (int i = 0; i < Patch::HEIGHT; i++) {
        for (int j = 0; j < Patch::WIDTH; j++) {
            float templateVariance = templateImg->at<float>(i,j);
            float sampleVariance = sampleImg->at<float>(i,j);

            corr += templateVariance*sampleVariance;
            normTemplate += templateVariance*templateVariance;
            normSample += sampleVariance*sampleVariance;
        }
    }

    float upper = corr;
    float lower = sqrt(normTemplate * normSample);
    float ncc = (upper / lower);
    return 0.5 * (ncc + 1);
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
