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
    string key(argv[1]);
    Sequence* sequence = new Sequence(key);
    Frame* firstFrame = sequence->next();
    Box* firstBox = sequence->initBox;

    printf("FIRST >>>> %s\n", firstBox->toCharArr());

    MeanVariance* initialMeanVariance = firstFrame->integral->computeMeanVariance(
        (int) firstBox->x1,
        (int) firstBox->y1,
        (int) firstBox->width,
        (int) firstBox->height);
    float initialVarianceLimit = 490.45 * 0.5;
    printf("Initial Variance Limit: %f\n", initialVarianceLimit);

    BoxIterator* boxIterator = new BoxIterator(firstFrame, firstBox, 10, 24);
}
