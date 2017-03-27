#ifndef WARP_H
#define WARP_H

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/core/types_c.h"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

class Warp {
public:
    static IplImage* warp(IplImage* img, double* h, double xMin, double yMin, double xMax, double yMax);
};

#endif
