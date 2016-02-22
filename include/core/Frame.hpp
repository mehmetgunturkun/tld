#ifndef FRAME_HPP
#define FRAME_HPP

#include "core/Image.hpp"

class Frame {
public:
    Mat* colored;
    Mat* grayscale;
    Mat* gaussian;
    Mat* integral;

    vector<Mat> flowPyramid;

    Frame(string fileName);
    int get(Point2f* point, int imageType = CV_LOAD_IMAGE_GAUSSIAN);
    int get(int x, int y, int imageType = CV_LOAD_IMAGE_GAUSSIAN);
};
#endif
