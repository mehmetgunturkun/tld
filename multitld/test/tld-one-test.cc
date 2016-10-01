    #include <iostream>
#include <fstream>

#include "testbase/Sequence.hpp"
#include "tld/TLD.hpp"

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("You should declare key for dataset (e.g. --key car)\n");
        return 1;
    }

    string key(argv[2]);
    printf("Going to run for %s\n", key.c_str());
    Sequence* sequence = new Sequence(key);
    Frame* firstFrame = sequence->next();

    Box* initBox = sequence->initBox;
    vector<Box*> boxList = { initBox };

    ofstream outputFile;
    outputFile.open (sequence->outputFile);
    printf("mc1000\n");
    TLD* tld = new TLD(firstFrame, boxList);
    printf("mc2000\n");

    int frameNo = 1;
    Frame* previous = firstFrame;
    printf("Started to run for %s\n", key.c_str());
    Frame* secondFrame = sequence->next();

    boxList = tld->track(firstFrame, secondFrame, boxList);

    ImageBuilder* builder = new ImageBuilder(secondFrame);
    Box* box = boxList[0];
    if (box != nullptr) {
        builder->withBox(box);
        outputFile << box->toString() << "\n";
    } else {
        outputFile << "Box(nan, nan, nan, nan)";
    }
    builder->display(0);

    outputFile.close();
    return EXIT_SUCCESS;
}
