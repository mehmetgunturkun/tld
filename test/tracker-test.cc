#include "core/FrameView.hpp"
#include "util/Dataset.hpp"
#include "tracker/Tracker.hpp"

int main(int argc, char** argv) {
    Dataset* dataset = new Dataset("car");

    Frame* firstFrame = dataset->next();
    Box* firstBox = dataset->initBox;

    Tracker* tracker = new Tracker();
    int frameNo = 1;
    while(dataset->hasNext()) {

        FrameView* view = new FrameView(firstFrame);
        view->addBox(firstBox, FrameView::BLUE);
        Image::imshow("tracker", view->underlying, 1);

        Frame* secondFrame = dataset->next();
        printf("---  #%3d. %s. Frame ---\n", frameNo, secondFrame->name.c_str());
        Option<Box>* trackResult = tracker->track(firstFrame, secondFrame, firstBox);
        if (trackResult->isEmpty()) {
            println("Failed!");
            return EXIT_FAILURE;
        } else {
            firstBox = trackResult->get();
            firstFrame = secondFrame;
        }
        frameNo += 1;
        printf("-------------------\n");
    }
    return EXIT_SUCCESS;
}
