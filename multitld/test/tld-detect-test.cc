#include "testbase/Sequence.hpp"
#include "tld/TLD.hpp"

void display(Frame* frame, vector<Box*> boxList) {
    int nrOfBoxes = (int) boxList.size();
    ImageBuilder* builder = new ImageBuilder(frame);
    for (int i = 0; i < nrOfBoxes; i++) {
        Box* b = boxList[i];
        if (b != nullptr) {
            builder->withBox(b);
        }
    }
    builder->display(10);
}

void trainForTesting(Detector* detector, Frame* frame) {
    vector<Box*> positiveData;
    positiveData.push_back(new Box(0, 142, 126, 232, 165));
    vector<ScoredBox*> positiveDataForNN = detector->score(frame, positiveData);

    vector<Box*> negativeData;
    negativeData.push_back(new Box(0, 2, 27, 64, 54));
    negativeData.push_back(new Box(1, 142, 55, 204, 82));
    negativeData.push_back(new Box(2, 24, 61, 86, 88));
    negativeData.push_back(new Box(3, 30, 61, 92, 88));
    negativeData.push_back(new Box(4, 36, 64, 98, 91));
    negativeData.push_back(new Box(5, 24, 66, 86, 93));
    negativeData.push_back(new Box(6, 173, 66, 235, 93));
    negativeData.push_back(new Box(7, 30, 72, 92, 99));
    negativeData.push_back(new Box(8, 13, 78, 75, 105));
    negativeData.push_back(new Box(9, 195, 78, 257, 105));
    negativeData.push_back(new Box(10, 181, 83, 243, 110));
    negativeData.push_back(new Box(11, 190, 86, 252, 113));
    negativeData.push_back(new Box(12, 204, 86, 266, 113));
    negativeData.push_back(new Box(13, 108, 89, 170, 116));
    negativeData.push_back(new Box(14, 78, 97, 140, 124));
    negativeData.push_back(new Box(15, 128, 97, 190, 124));
    negativeData.push_back(new Box(16, 41, 100, 103, 127));
    negativeData.push_back(new Box(17, 173, 103, 235, 130));
    negativeData.push_back(new Box(18, 142, 106, 204, 133));
    negativeData.push_back(new Box(19, 190, 106, 252, 133));
    negativeData.push_back(new Box(20, 212, 111, 274, 138));
    negativeData.push_back(new Box(21, 69, 114, 131, 141));
    negativeData.push_back(new Box(22, 111, 117, 173, 144));
    negativeData.push_back(new Box(23, 22, 120, 84, 147));
    negativeData.push_back(new Box(24, 198, 122, 260, 149));
    negativeData.push_back(new Box(25, 246, 134, 308, 161));
    negativeData.push_back(new Box(26, 30, 136, 92, 163));
    negativeData.push_back(new Box(27, 52, 136, 114, 163));
    negativeData.push_back(new Box(28, 237, 145, 299, 172));
    negativeData.push_back(new Box(29, 30, 148, 92, 175));
    negativeData.push_back(new Box(30, 103, 167, 165, 194));
    negativeData.push_back(new Box(31, 128, 170, 190, 197));
    negativeData.push_back(new Box(32, 178, 176, 240, 203));
    negativeData.push_back(new Box(33, 55, 178, 117, 205));
    negativeData.push_back(new Box(34, 83, 178, 145, 205));
    negativeData.push_back(new Box(35, 223, 181, 285, 208));
    negativeData.push_back(new Box(36, 5, 184, 67, 211));
    negativeData.push_back(new Box(37, 65, 25, 140, 57));
    negativeData.push_back(new Box(38, 58, 35, 133, 67));
    negativeData.push_back(new Box(39, 111, 52, 186, 84));
    negativeData.push_back(new Box(40, 88, 55, 163, 87));
    negativeData.push_back(new Box(41, 65, 61, 140, 93));
    negativeData.push_back(new Box(42, 144, 71, 219, 103));
    negativeData.push_back(new Box(43, 134, 85, 209, 117));
    negativeData.push_back(new Box(44, 28, 118, 103, 150));
    negativeData.push_back(new Box(45, 94, 124, 169, 156));
    negativeData.push_back(new Box(46, 213, 124, 288, 156));
    negativeData.push_back(new Box(47, 35, 137, 110, 169));
    negativeData.push_back(new Box(48, 226, 147, 301, 179));
    negativeData.push_back(new Box(49, 12, 154, 87, 186));
    negativeData.push_back(new Box(50, 213, 167, 288, 199));
    negativeData.push_back(new Box(51, 9, 177, 84, 209));
    negativeData.push_back(new Box(52, 127, 177, 202, 209));
    negativeData.push_back(new Box(53, 154, 177, 229, 209));
    negativeData.push_back(new Box(54, 25, 180, 100, 212));
    negativeData.push_back(new Box(55, 190, 180, 265, 212));
    negativeData.push_back(new Box(56, 217, 180, 292, 212));
    negativeData.push_back(new Box(57, 9, 187, 84, 219));
    negativeData.push_back(new Box(58, 65, 193, 140, 225));
    negativeData.push_back(new Box(59, 78, 200, 153, 232));
    negativeData.push_back(new Box(60, 102, 62, 192, 101));
    negativeData.push_back(new Box(61, 170, 70, 260, 109));
    negativeData.push_back(new Box(62, 126, 74, 216, 113));
    negativeData.push_back(new Box(63, 138, 78, 228, 117));
    negativeData.push_back(new Box(64, 166, 94, 256, 133));
    negativeData.push_back(new Box(65, 178, 94, 268, 133));
    negativeData.push_back(new Box(66, 86, 106, 176, 145));
    negativeData.push_back(new Box(67, 222, 166, 312, 205));
    negativeData.push_back(new Box(68, 162, 186, 252, 225));
    negativeData.push_back(new Box(69, 158, 194, 248, 233));
    negativeData.push_back(new Box(70, 93, 26, 201, 73));
    negativeData.push_back(new Box(71, 36, 40, 144, 87));
    negativeData.push_back(new Box(72, 16, 146, 124, 193));
    negativeData.push_back(new Box(73, 36, 156, 144, 203));
    negativeData.push_back(new Box(74, 108, 156, 216, 203));
    negativeData.push_back(new Box(75, 127, 156, 235, 203));
    negativeData.push_back(new Box(76, 204, 156, 312, 203));
    negativeData.push_back(new Box(77, 74, 175, 182, 222));
    negativeData.push_back(new Box(78, 136, 180, 244, 227));
    negativeData.push_back(new Box(79, 146, 180, 254, 227));
    negativeData.push_back(new Box(80, 132, 189, 240, 236));
    negativeData.push_back(new Box(81, 130, 2, 260, 59));
    negativeData.push_back(new Box(82, 48, 31, 178, 88));
    negativeData.push_back(new Box(83, 66, 37, 196, 94));
    negativeData.push_back(new Box(84, 77, 37, 207, 94));
    negativeData.push_back(new Box(85, 72, 48, 202, 105));
    negativeData.push_back(new Box(86, 101, 54, 231, 111));
    negativeData.push_back(new Box(87, 188, 72, 318, 129));
    negativeData.push_back(new Box(88, 19, 101, 149, 158));
    negativeData.push_back(new Box(89, 188, 135, 318, 192));
    negativeData.push_back(new Box(90, 14, 147, 144, 204));
    negativeData.push_back(new Box(91, 147, 164, 277, 221));
    negativeData.push_back(new Box(92, 30, 16, 186, 84));
    negativeData.push_back(new Box(93, 50, 16, 206, 84));
    negativeData.push_back(new Box(94, 52, 27, 240, 109));
    negativeData.push_back(new Box(95, 10, 44, 198, 126));
    negativeData.push_back(new Box(96, 2, 127, 190, 209));
    negativeData.push_back(new Box(97, 52, 143, 240, 225));
    negativeData.push_back(new Box(98, 77, 151, 265, 233));
    negativeData.push_back(new Box(99, 14, 62, 285, 180));
    vector<ScoredBox*> negativeDataForNN = detector->score(frame, negativeData);
    TrainingSet<ScoredBox> trainingSet4NN = TrainingSet<ScoredBox>(
        frame,
        positiveDataForNN,
        negativeDataForNN,
        2
    );

    detector->nnClassifier->train(trainingSet4NN, 0);
}

int main(int argc, char** argv) {
    Sequence* sequence = new Sequence("tld/car");
    Frame* firstFrame = sequence->next();
    Frame* secondFrame = sequence->next();

    Box* initBox = sequence->initBox;
    vector<Box*> boxList = { initBox };
    int modelId = 0;

    printf(" ----- training begin -----\n");
    TLD* tld = new TLD(firstFrame, boxList);
    printf(" ----- training end -----\n");

    Detector* detector = tld->detector;
    vector<ScoredBox*> scoredBoxList = detector->detect(secondFrame);
    vector<ScoredBox*> candidateBoxList;
    vector<ScoredBox*> detectedBoxList;

    int nrOfBoxes = scoredBoxList.size();
    for (int i = 0; i < nrOfBoxes; i++) {
        ScoredBox* scoredBox = scoredBoxList[i];
        if (scoredBox->isClassified("ensemble", modelId)) {
            candidateBoxList.push_back(scoredBox);
        }

        if (scoredBox->isClassified("nn", modelId)) {
            detectedBoxList.push_back(scoredBox);
        }
    }

    nrOfBoxes = candidateBoxList.size();
    printf("===== candidate ======\n");
    for (int i = 0; i < nrOfBoxes; i++) {
        ScoredBox* scoredBox = candidateBoxList[i];
        printf("%s\n", scoredBox->box->toCharArr());
    }

    printf("===== detected ======\n");
    nrOfBoxes = detectedBoxList.size();
    for (int i = 0; i < nrOfBoxes; i++) {
        ScoredBox* scoredBox = detectedBoxList[i];
        printf("%s\n", scoredBox->box->toCharArr());
    }
    return EXIT_SUCCESS;
}
