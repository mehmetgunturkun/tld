
#include "multitld/tracker/Tracker.hpp"

int main(int argc, char** argv) {
    Frame* firstFrame;

    Box* b1 = new Box(1, 10, 10, 100, 100);
    Box* b2 = new Box(1, 10, 10, 100, 100);
    Box* b3 = new Box(1, 10, 10, 100, 100);
    Box* b4 = new Box(1, 10, 10, 100, 100);

    Tracker* tracker = new Tracker();
    vector<Box*> boxList = {b1, b2, b3, b4};
    vector<tld::Point*> points = tracker->generatePoints(boxList, 4);
    int nrOfPoints = points.size();
    for (int i = 0; i < nrOfPoints; i++) {
        tld::Point* point = points[i];
        printf("%s\n", point->toString().c_str());
    }
    return EXIT_SUCCESS;
}
