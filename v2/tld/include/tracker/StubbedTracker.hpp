
#ifndef STUBBED_TRACKER_H
#define STUBBED_TRACKER_H


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
    vector<Box*> mockedBoxList;

    StubbedTracker(string fileName, int nrOfFrames);
    vector<Box*> track(Frame* prev, Frame* curr, vector<Box*> boxList);
};

#endif
