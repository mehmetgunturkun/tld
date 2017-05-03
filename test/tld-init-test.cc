#include <iostream>
#include <fstream>

#include "common/Arguments.hpp"
#include "core/Sequence.hpp"
#include "tld/TLD.hpp"

int main(int argc, char** args) {
    Arguments arguments = Arguments(argc, args);

    string key = arguments.getString("sequence");
    int limit = arguments.getIntOrDefault("limit", 0);

    printf("Going to run for %s\n", key.c_str());
    Sequence sequence = Sequence(key, 0, limit);
    Frame* firstFrame = sequence.next();
    Frame* secondFrame = sequence.next();

    Box* initBox = sequence.initBox;
    initBox->isValid = true;
    vector<Box*> boxList = { initBox };


    TLD* tld = new TLD();

    string stubFile = arguments.getString("stubFile");
    StubbedTracker* tracker = new StubbedTracker(stubFile, sequence.nrOfFrames);
    tld->tracker = tracker;

    vector<Box*> correctedBoxList = tld->init(firstFrame, boxList);

    vector<Box*> currentBoxList = tld->track(firstFrame, secondFrame, correctedBoxList);

    for (int i = 0; i < 1; i++) {
        Box* correctedBox = correctedBoxList[i];
        delete correctedBox;
    }

    for (int i = 0; i < 1; i++) {
        Box* correctedBox = currentBoxList[i];
        delete correctedBox;
    }

    delete firstFrame;
    delete secondFrame;
    delete tld;
    return EXIT_SUCCESS;
}
