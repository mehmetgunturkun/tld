#ifndef GAUSSIAN_H
#define GAUSSIAN_H

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/imgproc/imgproc.hpp"

#include "opencv2/core/types_c.h"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

class Gaussian {
public:
    static IplImage* blur(IplImage* img, int kernelSize, double sigma);
};
#endif
