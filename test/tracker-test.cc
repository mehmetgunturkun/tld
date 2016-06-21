#include "core/FrameView.hpp"
#include "util/Dataset.hpp"
#include "tracker/Tracker.hpp"

int main(int argc, char** argv) {
    Dataset* dataset = new Dataset("car");

    Frame* firstFrame = dataset->next();
    Box* firstBox = dataset->initBox;

    Tracker* tracker = new Tracker();

    while(dataset->hasNext()) {
        FrameView* view = new FrameView(firstFrame);
        view->addBox(firstBox, FrameView::BLUE);
        Image::imshow("tracker", view->underlying, 1);

        Frame* secondFrame = dataset->next();
        Option<Box>* trackResult = tracker->track(firstFrame, secondFrame, firstBox);
        if (trackResult->isEmpty()) {
            println("Failed!");
            return EXIT_FAILURE;
        } else {
            firstBox = trackResult->get();
            firstFrame = secondFrame;
        }
    }
    return EXIT_SUCCESS;
}
