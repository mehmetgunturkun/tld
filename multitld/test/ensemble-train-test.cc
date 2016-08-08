
#include "detector/Detector.hpp"
#include "testbase/Sequence.hpp"

int main(int argc, char** argv) {
    Sequence* sequence = new Sequence("car");

    Frame* frame = sequence->next();

    Box* initBox = sequence->initBox;
    vector<Box*> boxList = initBox->splitTwo();

    Box* b0 = boxList[0];
    Box* b1 = boxList[1];

    printf("Box1 >> %s\n", b0->toString().c_str());
    printf("Box2 >> %s\n", b1->toString().c_str());

    boxList = {b1, b0};


    Detector* detector = new Detector(frame, boxList);

    detector->init(frame, boxList);

    vector<ScoredBox*> scoredBoxList = detector->detect(frame);


    return EXIT_SUCCESS;
}
