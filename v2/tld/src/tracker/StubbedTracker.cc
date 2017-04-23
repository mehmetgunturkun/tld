
#include "tracker/StubbedTracker.hpp"

StubbedTracker::StubbedTracker(string fileName, int nrOfFrames) {
    currentFrame = 0;
    ifstream source(fileName);

    mockedBoxList.resize(nrOfFrames);

    string line;
    while (getline(source, line)) {
        Option<Box*> maybeBox;

        StringStream stream = StringStream(line, ',');

        int frameId = stoi(stream.next());
        double x1 = stod(stream.next());
        double y1 = stod(stream.next());
        double x2 = stod(stream.next());
        double y2 = stod(stream.next());

        if (isnan(x1) || isnan(x2) || isnan(y1) || isnan(y2)) {
            maybeBox = Option<Box*>();
        } else {
            Box* box = new Box(x1, y1, x2, y2);
            maybeBox = Option<Box*>(box);
        }

        if (maybeBox.isDefined()) {
            Box* box = maybeBox.get();
            //printf("MOCK TRACKER >> %s\n", box->toTLDString().c_str());
            mockedBoxList[frameId] = box;
        } else {
            mockedBoxList[frameId] = NULL;
        }
    }
}

vector<Box*> StubbedTracker::track(Frame* prev, Frame* curr, vector<Box*> boxList) {
    Box* inputBox = boxList[0];
    Box* outputBox = mockedBoxList[currentFrame];

    printf("TRACKER( IN): ");
    if (inputBox != NULL) {
        inputBox->print();
    } else {
        printf("\n");
    }

    printf("TRACKER(OUT): ");
    if (outputBox != NULL) {
        outputBox->print();
    } else {
        printf("\n");
    }


    currentFrame += 1;
    vector<Box*> nextBoxList = { outputBox };
    return nextBoxList;
}
