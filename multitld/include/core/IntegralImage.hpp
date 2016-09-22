#ifndef INTEGRAL_IMAGE_HPP
#define INTEGRAL_IMAGE_HPP

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
public:
    int step;
    int width;
    int height;
    int* integralImage;
    int* integralSquaredImage;
    int* computeIntegralImage(Mat* img, int (f)(int));
    int* computeIntegralImage(Mat* sourceImage);
    int* computeSquaredIntegralImage(Mat* sourceImage);

    IntegralImage(Mat* img);
    MeanVariance* computeMeanVariance(Box* box);
    MeanVariance* computeMeanVariance(int x, int y, int width, int height);
    int computeSubWindow(int row, int col, int pWidth, int pHeight, bool isSquared);
};
#endif
