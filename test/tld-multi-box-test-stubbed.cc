#include <iostream>
#include <fstream>

#include "common/Arguments.hpp"
#include "core/Sequence.hpp"
#include "tld/TLD.hpp"

int main(int argc, char** args) {
    Arguments arguments = Arguments(argc, args);

    string key = arguments.getString("sequence");
    int limit = arguments.getIntOrDefault("limit", NO_LIMIT);

    printf("Going to run for %s\n", key.c_str());
    Sequence sequence = Sequence(key, 0, limit);
    Frame* firstFrame = sequence.next();
    Box* initBox = sequence.initBox;
    initBox->isValid = true;

    initBox->x1 -= 1.0;
    initBox->y1 -= 1.0;
    initBox->x2 -= 1.0;
    initBox->y2 -= 1.0;

    Box* b0 = initBox->clone();
    Box* b1 = initBox->clone();
    Box* b2 = initBox->clone();

    vector<Box*> boxList = { b0, b1, b2 };
    int nrOfModels = (int) boxList.size();

    printf("%s\n", initBox->toString().c_str());

    //string stubFile = arguments.getString("stubFile");
    //StubbedTracker* tracker = new StubbedTracker(stubFile, sequence.nrOfFrames);

    TLD* tld = new TLD();
    //tld->tracker = tracker;

    vector<Box*> correctedBoxList = tld->init(firstFrame, boxList);

    Frame* previous = firstFrame;
    while (sequence.hasNext()) {
        Frame* current = sequence.next();
        printf("Frame(%d) >> Frame(%d)\n", previous->id, current->id);

        vector<Box*> currentCorrectedBoxList = tld->track(previous, current, correctedBoxList);
        ImageBuilder* builder = new ImageBuilder(current);

        for (int i = 0; i < nrOfModels; i++) {
            Box* box = currentCorrectedBoxList[i];

            if (box != nullptr) {
                // string boxString = box->toTLDString();
                builder->withBox(box);
            } else {
                printf("Frame(%d) >> Frame(%d) FAILED\n", previous->id, current->id);
            }
        }

        builder->display(1);

        for (int i = 0; i < nrOfModels; i++) {
            delete correctedBoxList[i];
        }

        correctedBoxList = currentCorrectedBoxList;

        delete previous;
        previous = current;
    }

    delete previous;
    delete tld;

    return EXIT_SUCCESS;
}
