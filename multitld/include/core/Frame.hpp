#ifndef FRAME_H
#define FRAME_H

#include "core/Image.hpp"

class Frame {
public:
    string name;
    Mat* displayImg;
    Mat* grayscale;
    vector<Mat> flowPyramid;

    Frame(string fileName);
    void show();
};

#endif
