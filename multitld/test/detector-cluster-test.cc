
#include "detector/ensemble/EnsembleClassifier.hpp"
#include "detector/Detector.hpp"
#include "testbase/Sequence.hpp"

//TODO Remove
void printScoreReport(vector<ScoredBox*> scoredBoxList) {
    // int nrOfBoxes = (int) scoredBoxList.size();
    // for (int i = 0 ; i < nrOfBoxes; i++) {
    //     ScoredBox* scoredBox = scoredBoxList[i];
    //
    //     EnsembleScore* ensembleScore = (EnsembleScore*) scoredBox->getScore("ensemble");
    //     int nrOfBinaryCodes = (int) ensembleScore->binaryCodes.size();
    //     printf("---------------------\n");
    //     printf("BinaryCodes x %d:\n", nrOfBinaryCodes);
    //     for (int j = 0; j < nrOfBinaryCodes; j++) {
    //         int binaryCode = ensembleScore->binaryCodes[j];
    //         printf("\t%2d. %5d\n", j, binaryCode);
    //     }
    //     int nrOfScores = (int) ensembleScore->scores.size();
    //     printf("Scores x %d:\n", nrOfScores);
    //     for (int j = 0; j < nrOfScores; j++) {
    //         float score = ensembleScore->scores[j];
    //         printf("\t%2d. %1.3f\n", j, score);
    //     }
    //
    //     NNScore* nnScore = (NNScore*) scoredBox->getScore("nn");
    //     printf("---------------------\n");
    //     printf("NNScores:\n");
    //     for (int j = 0; j < nrOfScores; j++) {
    //         float relativeScore = nnScore->relativeScores[j];
    //         float conservativeScore = nnScore->conservativeScores[j];
    //         printf("\t%2d. %1.3f, %1.3f\n", j, relativeScore, conservativeScore);
    //     }
    // }
}

int main(int argc, char** argv) {
    Sequence* sequence = new Sequence("car");

    Frame* frame = sequence->next();
    Frame* testFrame = sequence->next();

    Box* initBox = sequence->initBox;
    vector<Box*> boxList = { initBox };

    Detector* detector = new Detector(frame, boxList);

    detector->init(frame, boxList);

    vector<ScoredBox*> scoredBoxList = detector->detect(testFrame);
    vector<ScoredBox*> detectedBoxList;
    int nrOfBoxes = scoredBoxList.size();
    for (int i = 0 ; i < nrOfBoxes; i++) {
        ScoredBox* scoredBox = scoredBoxList[i];
        if (scoredBox->isDetected) {
            detectedBoxList.push_back(scoredBox);
        }
    }

    nrOfBoxes  = detectedBoxList.size();

    printScoreReport(detectedBoxList);
    vector<ScoredBox*> clusteredBoxList = ScoredBox::cluster(detectedBoxList, nrOfBoxes);
    printScoreReport(clusteredBoxList);

    return EXIT_SUCCESS;
}
