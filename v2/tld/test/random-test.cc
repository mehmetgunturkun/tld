
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <fstream>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/imgproc/imgproc.hpp"

#include "opencv2/core/types_c.h"
#include "opencv2/highgui/highgui.hpp"

#include "core/Box.hpp"
#include "core/Frame.hpp"
#include "core/Sequence.hpp"
#include "common/Arguments.hpp"

using namespace std;
using namespace cv;

#define margin 5.0

class StubbedTracker {
public:
    int currentFrame;
    ifstream source;

    StubbedTracker(string fileName) {
        currentFrame = 0;
        source.open(fileName);
    }

    vector<Box*> track(Frame* prev, Frame* curr, vector<Box*> boxList) {
        vector<Box*> nextBoxList;
        currentFrame += 1;

        string line;
        if (getline(source, line)) {
            Option<Box*> maybeBox = Box::fromLine(line);
            if (maybeBox.isDefined()) {
                Box* nextBox = maybeBox.get();
                printf("%s\n", nextBox->toTLDString().c_str());
                nextBoxList.push_back(maybeBox.get());
            } else {
                nextBoxList.push_back(NULL);
            }
        } else {
            printf("There is no line for Frame: %3d\n", currentFrame);
            nextBoxList.push_back(NULL);
        }

        return nextBoxList;
    }
};

int main(int argc, char** argv) {
    Arguments args = Arguments(argc, argv);

    string fileName = args.getString("stubFile");
    StubbedTracker* tracker = new StubbedTracker(fileName);

    string boxString = args.getString("box");
    Option<Box*> maybeBox = Box::fromLine(boxString);

    printf("mc2000\n");
    string line = "0x1.0c7dd712dc49cp+7,0x1.0ea3d5e74c39p+7,0x1.3748b7d550d9p+7,0x1.7a95cf0a6a52cp+7\n";
    Option<Box*> mBox = Box::fromLine(line);
    printf("mc2100\n");


    if (maybeBox.isDefined()) {
        Box* box = maybeBox.get();
        vector<Box*> boxList = { box };
        tracker->track(NULL, NULL, boxList);
        tracker->track(NULL, NULL, boxList);
        tracker->track(NULL, NULL, boxList);

    } else {
        println("There is no valid box in %s", boxString.c_str());
    }

}
