
#include "tracker/Tracker.hpp"
#include "testbase/Sequence.hpp"

int main(int argc, char** argv) {
    Box* b1 = new Box(1, 142, 125, 232, 165);

    Tracker* tracker = new Tracker();

    Sequence* sequence = new Sequence("car");
    Frame* firstFrame = sequence->next();

    Box* firstBox = sequence->initBox;
    vector<Box*> boxList = { firstBox, firstBox, firstBox };
    int frameNo = 1;
    Frame* previous = firstFrame;
    while (sequence->hasNext()) {
        Frame* current = sequence->next();
        printf("---  #%3d. %s. Frame ---\n", frameNo, current->name.c_str());
        boxList = tracker->track(previous, current, boxList);

        if (boxList.size() == 0) {
            return EXIT_FAILURE;
        }

        previous = current;
        printf("-------------------\n");
        frameNo += 1;
    }




    // vector<Box*> trackedPoints = tracker->track(firstFrame, secondFrame, boxList);

    return EXIT_SUCCESS;
}
