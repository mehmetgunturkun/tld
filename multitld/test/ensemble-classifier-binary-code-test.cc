#include "testbase/Sequence.hpp"
#include "common/BoundedPriorityQueue.hpp"
#include "core/BoxIterator.hpp"
#include "util/ImageBuilder.hpp"

#include "detector/ensemble/EnsembleClassifier.hpp"


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
    vector<Box*> boxList = { firstBox };
    EnsembleClassifier* eClassifier = new EnsembleClassifier(firstFrame, boxList);

    vector<BaseClassifier*> classifierList = eClassifier->baseClassifiers;
    for (int i = 0; i < classifierList.size(); i++) {
        classifierList[i]->printPixelComparison();
    }

    Box* b = new Box(0, 141, 125, 231, 164);
    ScoredBox* sb = new ScoredBox(b);
    eClassifier->score(firstFrame, sb);

}
