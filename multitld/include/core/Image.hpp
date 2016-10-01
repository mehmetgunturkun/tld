#ifndef IMAGE_H
#define IMAGE_H

#define CV_LOAD_IMAGE_GAUSSIAN -1
#include <iostream>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/tracking.hpp"

#include "core/Random.hpp"

using namespace std;
using namespace cv;

class Image {
public:
    int width;
    int height;
    int nrOfChannels;
    Mat* underlying;
    Image(Mat* img);

    static Image* rgb2gray(Image* image);
    static Image* bgr2gray(Image* image);
};
#endif
