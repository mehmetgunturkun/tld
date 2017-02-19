#ifndef FRAME_H
#define FRAME_H

#include "core/Image.hpp"
#include "core/Gaussian.hpp"
#include "core/IntegralImage.hpp"

class Frame {
public:
    int id;
    string name;

    int width;
    int height;

    Mat* displayImg;
    Mat* grayscale;
    Mat* gaussian;
    IntegralImage* integral;
    vector<Mat> flowPyramid;

    Frame(string fileName);
    Frame(Mat* grayscale, Mat* gaussian);
    Frame() {}

    int get(Point2f* point, int imageType = CV_LOAD_IMAGE_GAUSSIAN);
    int get(int x, int y, int imageType = CV_LOAD_IMAGE_GAUSSIAN);
    Frame* clone();

    static Frame* warp(Frame* frame, Box* box);
    static void dealloc(Frame* frame);


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
};

#endif
