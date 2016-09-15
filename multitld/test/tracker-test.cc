
#include "tracker/Tracker.hpp"
#include "testbase/Sequence.hpp"
#include "common/Colors.hpp"
#include "util/ImageBuilder.hpp"

void display(Frame* frame, vector<Box*> boxList) {
    int nrOfBoxes = (int) boxList.size();
    ImageBuilder* builder = new ImageBuilder(frame);
    for (int i = 0; i < nrOfBoxes; i++) {
        Box* b = boxList[i];
        if (b != nullptr) {
            builder->withBox(b);
        }
    }
    builder->display(100);
}

int main(int argc, char** argv) {
    Sequence* sequence = new Sequence("car");
    Frame* firstFrame = sequence->next();

    Box* initBox = sequence->initBox;
    vector<Box*> boxList = { initBox };

    Tracker* tracker = new Tracker();

    int frameNo = 1;
    Frame* previous = firstFrame;
    while (sequence->hasNext()) {
        Frame* current = sequence->next();
        frameNo += 1;

        boxList = tracker->track(previous, current, boxList);
        display(current, boxList);
        previous = current;
    }

    return EXIT_SUCCESS;
}
