
#include "detector/nn/NearestNeighborClassifier.hpp"
#include "detector/Detector.hpp"
#include "testbase/Sequence.hpp"

int main(int argc, char** argv) {
    Sequence* sequence = new Sequence("tld/car");

    Frame* frame = sequence->next();
    Frame* testFrame = sequence->next();

    Box* initBox = sequence->initBox;
    vector<Box*> boxList = { initBox };

    Detector* detector = new Detector(frame, boxList);
    NearestNeighborClassifier* nnClassifier = detector->nnClassifier;

    // eClassifier->dumpEnsembleClassifier();
    // detector->init(frame, boxList);
    // eClassifier->dumpEnsembleClassifier();

    // vector<ScoredBox*> scoredBoxList = detector->detect(testFrame);
    // int nrOfBoxes = scoredBoxList.size();
    // for (int i = 0 ; i < nrOfBoxes; i++) {
    //     ScoredBox* scoredBox = scoredBoxList[i];
    //     if (scoredBox->isDetected) {
    //         ImageBuilder* builder = (new ImageBuilder(frame))->withBox(scoredBox->box);
    //         EnsembleScore* ensembleScore = (EnsembleScore*) scoredBox->getScore("ensemble");
    //         float score = ensembleScore->scores[0];
    //         printf("Score >> %3.3f\n", score);
    //         builder->display(0);
    //     }
    // }
    return EXIT_SUCCESS;
}
