#include "util/Dataset.hpp"
#include "tracker/Tracker.hpp"

int main(int argc, char** argv) {
    Dataset* dataset = new Dataset("car");

    Frame* firstFrame = dataset->next();
    Box* firstBox = dataset->initBox;

    Tracker* tracker = new Tracker();

    while(dataset->hasNext()) {
        printf("BOX >> %s\n", firstBox->toString().c_str());
        Frame* secondFrame = dataset->next();
        Option<Box>* trackResult = tracker->track(firstFrame, secondFrame, firstBox);
        if (trackResult->isEmpty()) {
            return EXIT_FAILURE;
        } else {
            firstBox = trackResult->get();
            firstFrame = secondFrame;
        }
    }
    return EXIT_SUCCESS;
}
