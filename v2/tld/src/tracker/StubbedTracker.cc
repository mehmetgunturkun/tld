
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

StubbedTracker::~StubbedTracker() {
    int nrOfFrames = (int) mockedBoxList.size();
    for (int i = 0; i < nrOfFrames; i++) {
        Box* b = mockedBoxList[i];
        if (b != NULL) {
            delete b;
        }
    }
}

vector<Box*> StubbedTracker::track(Frame* prev, Frame* curr, vector<Box*> boxList) {
    int nrOfModels = (int) boxList.size();

    Box* inputBox = boxList[0];
    Box* outputBox = mockedBoxList[currentFrame];

    printf("TRACKER( IN): ");
    if (inputBox != NULL) {
        inputBox->print();
    } else {
        printf("\n");
    }

    vector<Box*> nextBoxList;
    printf("TRACKER(OUT): ");
    if (outputBox != NULL) {
        Box* clonedOutputBox = outputBox->clone();
        for (int i = 0; i < nrOfModels; i++) {
            nextBoxList.push_back(clonedOutputBox);
        }
        outputBox->print();
    } else {
        for (int i = 0; i < nrOfModels; i++) {
            nextBoxList.push_back(NULL);
        }
        printf("\n");
    }


    currentFrame += 1;
    return nextBoxList;
}
