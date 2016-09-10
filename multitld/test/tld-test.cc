#include "testbase/Sequence.hpp"
#include "tld/TLD.hpp"

int main(int argc, char** argv) {
    Sequence* sequence = new Sequence("car");
    Frame* firstFrame = sequence->next();

    Box* initBox = sequence->initBox;
    vector<Box*> boxList = { initBox };

    TLD* tld = new TLD(firstFrame, boxList);

    int frameNo = 1;
    Frame* previous = firstFrame;
    while (sequence->hasNext()) {
        Frame* current = sequence->next();
        frameNo += 1;

        boxList = tld->track(previous, current, boxList);

        printf("Frame#%d: %s is processing...\n", frameNo, current->name.c_str());

        previous = current;

    }
    return EXIT_SUCCESS;
}
