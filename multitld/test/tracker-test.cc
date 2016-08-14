
#include "tracker/Tracker.hpp"
#include "testbase/Sequence.hpp"
#include "common/Colors.hpp"
#include "util/ImageBuilder.hpp"

int main(int argc, char** argv) {
    Sequence* sequence = new Sequence("car");
    Frame* firstFrame = sequence->next();
    Frame* secondFrame = sequence->next();

    Box* initBox = sequence->initBox;
    vector<Box*> boxList = initBox->splitTwo();
    boxList = { initBox, nullptr, boxList[0], boxList[1] };

    Tracker* tracker = new Tracker();

    vector<Box*> nextBoxList = tracker->track(firstFrame, secondFrame, boxList);
    for (int i = 0; i < 4; i++) {
        Box* box = nextBoxList[i];
        if (box == nullptr) {
            printf("BOX >> Failed\n");
        } else {
            printf("BOX >> %s\n", box->toString().c_str());
        }
    }
    return EXIT_SUCCESS;
}
