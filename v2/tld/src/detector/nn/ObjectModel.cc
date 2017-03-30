#include "detector/nn/ObjectModel.hpp"

ObjectScore::ObjectScore(double relativeScore, double conservativeScore, bool isInPositive, bool isInNegative, int closestPositivePatchIndex) {
    this->relativeScore = relativeScore;
    this->conservativeScore = conservativeScore;
    this->isInPositive = isInPositive;
    this->isInNegative = isInNegative;
    this->closestPositivePatchIndex = closestPositivePatchIndex;
}

ObjectModel::ObjectModel() {
    nrOfPositivePatches = 0;
    nrOfNegativePatches = 0;
}

ObjectScore* ObjectModel::computeScore(Patch* sample) {
    if (nrOfPositivePatches == 0) {
        ObjectScore* objectScore = new ObjectScore(0.0, 0.0, false, false, 0);
        return objectScore;
    }

    if (nrOfNegativePatches == 0) {
        ObjectScore* objectScore = new ObjectScore(1.0, 1.0, false, false, 0);
        return objectScore;
    }

    tuple<double, int> positiveNNTuple = computePositiveSimilarity(sample);
    tuple<double, int> positiveInititalNNTuple = computeInitialPositiveSimilarity(sample);
    tuple<double, int> negativeNNTuple = computeNegativeSimilarity(sample);

    double positiveNNSimilarity = get<0>(positiveNNTuple);
    double positiveInititalNNSimilarity = get<0>(positiveInititalNNTuple);
    double negativeNNSimilarity = get<0>(negativeNNTuple);

    int closestPositivePatchIndex = get<1>(positiveNNTuple);

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

    ObjectScore* objectScore = new ObjectScore(
        relativeScore,
        conservativeScore,
        isInPositive,
        isInNegative,
        closestPositivePatchIndex
    );
    return objectScore;
}

tuple<double, int> ObjectModel::computeInitialPositiveSimilarity(Patch* sample) {
    int size = (int) ceil( (double) nrOfPositivePatches / 2);
    return computeNearestSimilarity(positivePatchList, size, sample);
}

tuple<double, int> ObjectModel::computePositiveSimilarity(Patch* sample) {
    return computeNearestSimilarity(positivePatchList, nrOfPositivePatches, sample);
}

tuple<double, int> ObjectModel::computeNegativeSimilarity(Patch* sample) {
    return computeNearestSimilarity(negativePatchList, nrOfNegativePatches, sample);
}

tuple<double, int> ObjectModel::computeNearestSimilarity(vector<Patch*> templatePatchList, int size, Patch* sample) {
    double similarityValue = 0.0;
    int index = size;
    for (int i = 0; i < size; i++) {
            Patch* templatePatch = templatePatchList[i];
            double tempSimilarityValue = computeSimilarity(templatePatch, sample);
            if (tempSimilarityValue > similarityValue) {
                    similarityValue = tempSimilarityValue;
                    index = i + 1;
            }
    }

    if (index > size) {
        index = size;
    }

    similarityValue = 1.0 - similarityValue;
    return make_tuple(similarityValue, index);
}

double ObjectModel::computeSimilarity(Patch* templatePatch, Patch* samplePatch) {
    IplImage* templateImg = templatePatch->data;
    IplImage* sampleImg = samplePatch->data;

    double corr = 0.0;
    double normTemplate = 0.0;
    double normSample = 0.0;

    for (int i = 0; i < Patch::HEIGHT; i++) {
        for (int j = 0; j < Patch::WIDTH; j++) {

            double templateVariance = CV_IMAGE_GET_ELEM(templateImg, double, i, j);
            double sampleVariance = CV_IMAGE_GET_ELEM(sampleImg, double, i, j);

            corr += templateVariance*sampleVariance;
            normTemplate += templateVariance*templateVariance;
            normSample += sampleVariance*sampleVariance;
        }
    }

    double upper = corr;
    double lower = sqrt(normTemplate * normSample);
    double ncc = (upper / lower);
    return (ncc + 1.0) / 2.0;
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

void ObjectModel::add(Patch* patch, int index, bool label) {
    if (label) {
        positivePatchList.insert(positivePatchList.begin() + index, patch);
        nrOfPositivePatches += 1;
    } else {
        negativePatchList.insert(negativePatchList.begin() + index, patch);
        nrOfNegativePatches += 1;
    }
}

void ObjectModel::replace(Patch* patch, int oldIndex) {
    // this->add(patch, true);
}
