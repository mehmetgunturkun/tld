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
    Box* initBox = sequence.initBox;
    initBox->isValid = true;
    vector<Box*> boxList = { initBox };
    printf("%s\n", initBox->toString().c_str());

    TLD* tld = new TLD();
    boxList = tld->init(firstFrame, boxList);

    Frame* previous = firstFrame;
    while (sequence.hasNext()) {
        Frame* current = sequence.next();
        printf("Frame(%d) >> Frame(%d)\n", previous->id, current->id);

        boxList = tld->track(previous, current, boxList);

        Box* box = boxList[0];
        ImageBuilder* builder = new ImageBuilder(current);
        if (box != nullptr) {
            // string boxString = box->toTLDString();
            builder->withBox(box);
        } else {
            printf("Frame(%d) >> Frame(%d) FAILED\n", previous->id, current->id);
        }
        builder->display(1);
        previous = current;
    }

    return EXIT_SUCCESS;
}
