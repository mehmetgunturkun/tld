#include <iostream>
#include <fstream>

#include "common/Arguments.hpp"

#include "testbase/Sequence.hpp"
#include "tld/TLD.hpp"

int main(int argc, char** argv) {
    Arguments* args = new Arguments(argc, argv);

    string key = args->getString("sequence");
    printf("Going to run for %s\n", key.c_str());
    Sequence* sequence = new Sequence(key);
    Frame* firstFrame = sequence->next();

    Box* initBox = sequence->initBox;
    vector<Box*> boxList = initBox->splitTwo();

    TLD* tld = new TLD(firstFrame, boxList);
    boxList = tld->init();
    
    int frameNo = 1;
    Frame* previous = firstFrame;
    printf("Started to run for %s\n", key.c_str());
    while (sequence->hasNext()) {
        Frame* current = sequence->next();
        frameNo += 1;

        boxList = tld->track(previous, current, boxList);
        ImageBuilder* builder = new ImageBuilder(current);
        for (int i = 0; i < 2; i++) {
            Box* box = boxList[i];
            if (box != nullptr) {
                builder->withBox(box);
            } else {
            }
        }
        builder->display(1);
        previous = current;
    }
    return EXIT_SUCCESS;
}
