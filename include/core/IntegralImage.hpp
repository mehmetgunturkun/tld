#ifndef INTEGRAL_IMAGE_HPP
#define INTEGRAL_IMAGE_HPP

#include <stdio.h>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "core/Box.hpp"

using namespace std;
using namespace cv;

class MeanVariance {
public:
    float mean;
    float variance;
    MeanVariance(float m, float v);
};

class IntegralImage {
private:
    int step;
    int width;
    int height;
    int* integralImage;
    int* integralSquaredImage;
    int* computeIntegralImage(IplImage* img, int (f)(int));
    int* computeIntegralImage(IplImage* sourceImage);
    int* computeSquaredIntegralImage(IplImage* sourceImage);

    int computeSubWindow(int row, int col, int pWidth, int pHeight, bool isSquared);
public:
    IntegralImage(IplImage* img);
    ~IntegralImage();

    MeanVariance* computeMeanVariance(Box* box);
    MeanVariance* computeMeanVariance(int x, int y, int width, int height);
};
#endif
