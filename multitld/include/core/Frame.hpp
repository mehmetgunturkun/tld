#ifndef FRAME_H
#define FRAME_H

#include "core/Image.hpp"
#include "core/IntegralImage.hpp"

class Frame {
public:
    string name;

    int width;
    int height;

    Mat* displayImg;
    Mat* grayscale;
    IntegralImage* integral;
    vector<Mat> flowPyramid;

    Frame(string fileName);
    // Frame(Mat* grayscale, Mat* gaussian);

    // int get(Point2f* point, int imageType = CV_LOAD_IMAGE_GAUSSIAN);
    // int get(int x, int y, int imageType = CV_LOAD_IMAGE_GAUSSIAN);

    string toString() {
        stringstream ss;
        ss  << "Frame("
            << name
            << ", "
            << width
            << ", "
            << height
            << ")";
        return ss.str();
    }
    void show();
};

#endif