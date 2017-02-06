#include <iostream>
#include <fstream>

#include "common/Arguments.hpp"
#include "testbase/Sequence.hpp"
#include "tld/TLD.hpp"

int main(int argc, char** args) {
    Arguments* arguments = new Arguments(argc, args);

    string key = arguments->getString("sequence");
    int limit = arguments->getIntOrDefault("limit", 0);

    printf("Going to run for %s\n", key.c_str());
    Sequence* sequence = new Sequence(key, 0, limit);
    Frame* firstFrame = sequence->next();

    Box* initBox = sequence->initBox;
    initBox->isValid = true;
    vector<Box*> boxList = { initBox };

    ofstream outputFile;
    outputFile.open (sequence->outputFile);

    TLD* tld = new TLD(firstFrame, boxList);
    boxList = tld->init();
    tld->detector->dumpDetector();

    int frameNo = 0;
    Frame* previous = firstFrame;
    outputFile << initBox->toTLDString() << "\n";
    printf("Started to run for %s\n", key.c_str());

    while (sequence->hasNext()) {
        printf("Frame(%d) >> Frame(%d)\n", frameNo, frameNo + 1);
        Frame* current = sequence->next();
        frameNo += 1;

        boxList = tld->track(previous, current, boxList);
        Box* box = boxList[0];
        ImageBuilder* builder = new ImageBuilder(current);
        if (box != nullptr) {
            string boxString = box->toTLDString();
            builder->withBox(box);
            outputFile << boxString << "\n";
        } else {
            outputFile << "nan, nan, nan, nan\n";
        }
        builder->display(1);
        previous = current;
    }

    tld->detector->dumpDetector();

    outputFile.close();
    return EXIT_SUCCESS;
}
