#include "common/Arguments.hpp"
#include "core/Frame.hpp"
#include "core/Sequence.hpp"

#include "detector/Detector.hpp"

int main(int argc, char** argv) {
    Arguments args = Arguments(argc, argv);

    string sequenceKey = args.getString("sequence");
    Sequence sequence = Sequence(sequenceKey);

    Frame* firstFrame = sequence.next();
    Frame* secondFrame = sequence.next();
    Box* firstBox = sequence.initBox;
    vector<Box*> boxList = { firstBox };

    Detector* detector = new Detector();

    //Initialization
    vector<Box*> correctedBoxList = detector->init(firstFrame, boxList);

    for (int i = 0; i < (int) correctedBoxList.size(); i++) {
        Box* correctedBox = correctedBoxList[i];
        delete correctedBox;
    }

    //Detection
    vector<ScoredBox*> allBoxList = detector->detect(firstFrame);

    //Learning
    printf("Learning....\n");
    detector->learn(secondFrame, firstBox, allBoxList, 0);

    for (int i = 0; i < (int) allBoxList.size(); i++) {
        ScoredBox* sb = allBoxList[i];
        delete sb;
    }

    //Get Closest Box
    Box* theClosestBox = detector->getClosestBox(firstFrame, firstBox);
    delete theClosestBox;

    //Validation
    Box* anyBox = firstBox->clone();
    ScoredBox* validateBox = detector->validate(firstFrame, anyBox, 0);
    delete validateBox;

    //Evaluate
    bool evaluationResult = detector->evaluate(firstFrame, firstBox, 0);

    delete firstFrame;
    delete secondFrame;
    delete detector;

}
