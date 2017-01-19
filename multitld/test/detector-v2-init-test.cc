#include "testbase/Sequence.hpp"
#include "common/Arguments.hpp"
#include "detector/Detector.hpp"

int main(int argc, char** args) {
    Arguments* arguments = new Arguments(argc, args);
    string sequenceKey = arguments->getString("sequence");
    Sequence* sequence = new Sequence(sequenceKey);

    Frame* firstFrame = sequence->next();
    Box* initBox = sequence->initBox;

    vector<Box*> boxList = { initBox };
    Detector* detector = new Detector(firstFrame, boxList);
    detector->init(firstFrame, boxList);
}
