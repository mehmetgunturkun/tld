
#include "detector/Detector.hpp"
#include "testbase/Sequence.hpp"

int main(int argc, char** argv) {
    Sequence* sequence = new Sequence("car");

    Frame* frame = sequence->next();

    Box* initBox = sequence->initBox;

    Box* b0 = initBox->move(0.0f, 0.0f);
    Box* b1 = initBox->move(50.0f, 50.0f);
    Box* b2 = initBox->move(-50.0f, 50.0f);
    Box* b3 = initBox->move(50.0f, -50.0f);
    Box* b4 = initBox->move(-50.0f, -50.0f);


    vector<Box*> boxList = { b0, b1, b2, b3, b4 };
    Detector* detector = new Detector(frame, boxList);
    printf("Init Box: %s\n", initBox->toString().c_str());
    detector->init(frame, initBox);

    vector<ScoredBox*> scoredBoxList = detector->detect(frame);


    return EXIT_SUCCESS;
}
