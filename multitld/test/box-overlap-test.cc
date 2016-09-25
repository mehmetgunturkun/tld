#include "testbase/Sequence.hpp"
#include "common/BoundedPriorityQueue.hpp"
#include "core/BoxIterator.hpp"
#include "util/ImageBuilder.hpp"


#include <iostream>
#include <string>
#include <math.h>

struct OverlapOrderedV2 {
  bool operator() (Box* box1, Box* box2) {
      return box1->overlap > box2->overlap;
  }
};

int main(int argc, char** argv) {
    Sequence* sequence = new Sequence("tld/car");
    Frame* firstFrame = sequence->next();
    Box* firstBox = sequence->initBox;

    MeanVariance* initialMeanVariance = firstFrame->integral->computeMeanVariance(
        (int) firstBox->x1,
        (int) firstBox->y1,
        (int) firstBox->width,
        (int) firstBox->height);
    float initialVarianceLimit = 453.45 * 0.5;
    printf("Initial Variance Limit: %f\n", initialVarianceLimit);

    BoxIterator* boxIterator = new BoxIterator(firstFrame, firstBox, 10, 24);

    BoundedPriorityQueue<Box, OverlapOrderedV2> positiveQueue = BoundedPriorityQueue<Box, OverlapOrderedV2>(100);
    vector<Box*> negativeQueue;

    int nrOfTotalBoxes = 0;
    while (boxIterator->hasNext()) {
        Box* box = boxIterator->next();
        nrOfTotalBoxes += 1;
        float overlap = Box::computeOverlap(firstBox, box);
        box->overlap = overlap;
        if (overlap > 0.6) {
            positiveQueue += box;
        }

        if (overlap < 0.2) {
            negativeQueue.push_back(box);
        }
    }

    vector<Box*> positiveBoxList = positiveQueue.toVector();
    int nrOfPositiveBoxes = (int) positiveBoxList.size();

    vector<Box*> negativeBoxList = negativeQueue;
    int nrOfNegativeBoxes = (int) negativeBoxList.size();

    printf("%5d positive patches\n", nrOfPositiveBoxes);
    printf("%5d negative patches\n", nrOfNegativeBoxes);
    printf("-----------------------\n");
    printf("%5d total patches\n", nrOfTotalBoxes);

    vector<Box*> negativeBoxListWithHighVariance;
    for (int i = 0; i < nrOfNegativeBoxes; i++) {
        Box* box = negativeBoxList[i];
        MeanVariance* meanVariance = firstFrame->integral->computeMeanVariance(
            (int) box->x1,
            (int) box->y1,
            (int) box->width,
            (int) box->height);
        float varianceValue = meanVariance->variance;
        if (varianceValue >= initialVarianceLimit) {
            negativeBoxListWithHighVariance.push_back(box);
        }
    }
    int nrOfNegativeBoxesWithHighVariance = (int) negativeBoxListWithHighVariance.size();
    printf("%5d negative patches has high variance\n", nrOfNegativeBoxesWithHighVariance);
    for (int i = 0; i < nrOfNegativeBoxesWithHighVariance; i++) {
        Box* box = negativeBoxListWithHighVariance[i];
        printf("%s\n", box->toCharArr());
    }


    vector<Box*> sampledNegativeBoxList = Random::randomSample(negativeBoxList, 100);
    int nrOfSampleNegativeBoxes = (int) sampledNegativeBoxList.size();


}
