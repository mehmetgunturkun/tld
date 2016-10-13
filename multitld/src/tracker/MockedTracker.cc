#include "tracker/Tracker.hpp"
#include "common/Colors.hpp"
#include <iostream>
#include <fstream>

vector<Box*> mockedBoxList;

Option<Box>* parseFromLine(string line) {
    StringStream* stream = new StringStream(line, ',');

    double x1 = stod(stream->next());
    double y1 = stod(stream->next());
    double x2 = stod(stream->next());
    double y2 = stod(stream->next());
    double score = stod(stream->next());

    if (isnan(x1) || isnan(x2) || isnan(y1) || isnan(y2)) {
        Option<Box>* none = new Option<Box>();
        return none;
    }

    Box* box = new Box(0, x1 - 1, y1 - 1, x2 - 1, y2 - 1);
    box->mockedScore = score;
    Option<Box>* maybeBox = new Option<Box>(box);
    return maybeBox;
}

Tracker::Tracker() {
    ifstream initFile("/tmp/mockedBoxList.csv");
    string line;
    while (getline(initFile, line)) {
        Option<Box>* maybeBox = parseFromLine(line);
        if (maybeBox->isDefined()) {
            mockedBoxList.push_back(maybeBox->get());
        } else {
            mockedBoxList.push_back(nullptr);
        }
    }

    // Load Predefined Box List
}

vector<Box*> Tracker::track(Frame* prev, Frame* curr, vector<Box*> boxList) {
    printf("Returning mocked result for %d\n", curr->id);
    Box* nextBox = mockedBoxList[curr->id];
    vector<Box*> nextBoxList;
    if (nextBox == nullptr) {
        nextBoxList =  { nullptr };
    } else {
        printf("TRACKER >>> %s, %g\n", nextBox->toCharArr(), nextBox->mockedScore);
        nextBoxList =  { nextBox };
    }
    return nextBoxList;
}
