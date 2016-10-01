
#include "detector/ensemble/EnsembleClassifier.hpp"
#include "detector/Detector.hpp"
#include "testbase/Sequence.hpp"

int main(int argc, char** argv) {
    Sequence* sequence = new Sequence("tld/car");

    Frame* frame = sequence->next();
    Frame* testFrame = sequence->next();

    Box* initBox = sequence->initBox;
    vector<Box*> boxList = { initBox };

    Detector* detector = new Detector(frame, boxList);
    EnsembleClassifier* eClassifier = detector->eClassifier;

    // eClassifier->dumpEnsembleClassifier();
    // detector->init(frame, boxList);

    vector<ScoredBox*> scoredBoxList = detector->detect(testFrame);

    // vector<ScoredBox*> scoredBoxList = detector->detect(testFrame);

    int nrOfBoxes = scoredBoxList.size();
    printf("NrOfBox: %d\n", nrOfBoxes);
    for (int i = 0 ; i < nrOfBoxes; i++) {
        ScoredBox* scoredBox = scoredBoxList[i];
        if (scoredBox->isDetected) {
            ImageBuilder* builder = (new ImageBuilder(frame))->withBox(scoredBox->box);
            EnsembleScore* ensembleScore = (EnsembleScore*) scoredBox->getScore("ensemble");
            float score = ensembleScore->scores[0];
            printf("%s\n", scoredBox->box->toCharArr());

            NNScore* nnScore = (NNScore*) scoredBox->getScore("nn");
            

            printf("Score >> %3.3f\n", score);
            // builder->display(0);
        }
    }
    return EXIT_SUCCESS;
}
