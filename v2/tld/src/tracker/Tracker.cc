#include "tracker/Tracker.hpp"

Tracker::~Tracker() {

}

vector<Box*> Tracker::track(Frame* prev, Frame* curr, vector<Box*> boxList) {
    // filter stable boxes
    int nrOfBoxes = (int) boxList.size();
    vector<Box*> estimatedBoxList;
    for (int i = 0; i < nrOfBoxes; i++) {
        estimatedBoxList.push_back(NULL);
    }
    return estimatedBoxList;
}
