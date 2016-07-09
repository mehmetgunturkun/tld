#include "detector/Detector.hpp"

Detector::Detector(Frame* frame, vector<Box*> boxList) {
    firstBox = boxList[0];
    vClassifier = new VarianceClassifier(frame, boxList);
    // eClassifier = new EnsembleClassifier();
    // nnClassifier = new NearestNeighborClassifier();
}

vector<ScoredBox*> Detector::detect(Frame* frame) {
    vector<ScoredBox*> allBoxList;
    BoxIterator* iterator = new BoxIterator(frame, firstBox, maxScaleLimit, minimumPatchSize);
    while (iterator->hasNext()) {
        Box* nextBox = iterator->next();
        if (!vClassifier->classify(frame, nextBox)) {
            continue;
        }

        ScoredBox* scoredBox = new ScoredBox(nextBox);
        allBoxList.push_back(scoredBox);
        // if (!eClassifier->classify(frame, scoredBox)) {
        //     continue;
        // }

        // if (!nnClassifier->classify(frame, scoredBox)) {
        //     continue;
        // }
    }
    return allBoxList;
}
