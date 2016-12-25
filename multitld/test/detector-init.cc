#include "testbase/Sequence.hpp"
#include "detector/Detector.hpp"

int main(int argc, char** args) {
    Sequence* sequence = new Sequence("06_car");

    Frame* firstFrame = sequence->next();
    Box* initBox = sequence->initBox;

    vector<Box*> boxList = { initBox };
    Detector* detector = new Detector(firstFrame, boxList);
    detector->init(firstFrame, boxList);
}
