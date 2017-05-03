#ifndef IMAGE_H
#define IMAGE_H

#define CV_LOAD_IMAGE_GAUSSIAN -1
#include <iostream>

#include "opencv2/core/types_c.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/highgui/highgui.hpp"

#include "core/Warp.hpp"
#include "common/Random.hpp"

using namespace std;
using namespace cv;

#define CV_IMAGE_GET_ELEM( image, elemtype, row, col )       \
    CV_IMAGE_ELEM( image, elemtype, row, col )

#define CV_IMAGE_SET_ELEM( image, elemtype, row, col, elem )       \
    elemtype* loc = ((elemtype*)((image)->imageData + (image)->widthStep*(row))); \
    loc[(col)] = elem

class Image {
public:
    static IplImage* imread(string fileName);
    static IplImage* imread(const char* fileName);
    static IplImage* imread(string fileName, int imageType);
    static IplImage* imread(const char* fileName, int imageType);
    static IplImage* rgb2gray(IplImage* image);
    static IplImage* bgr2gray(IplImage* image);
    static IplImage* warp(IplImage* img,
        double x1,
        double y1,
        double x2,
        double y2,
        double angle,
        double scale,
        double shift);
};
#endif
