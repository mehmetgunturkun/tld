#ifndef IMAGE_H
#define IMAGE_H

#define CV_LOAD_IMAGE_GAUSSIAN -1

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/tracking.hpp"

using namespace std;
using namespace cv;

class Image {
public:
    static Mat* imread(string fileName) {
        Mat tmpImage = cv::imread(fileName);
        Mat* image = new Mat(tmpImage);
        return image;
    }

    static Mat* imread(string fileName, int imageType) {
        Mat tmpImage = cv::imread(fileName, imageType);
        Mat* image = new Mat(tmpImage);
        return image;
    }

    static void imshow(string title, Mat* image, int delay = 0) {
        cv::imshow(title, *image);
        cv::waitKey(delay);
    }

    static Mat* rgb2gray(Mat* image) {
        Mat* dest = new Mat(*image);
        cv::cvtColor(*image, *dest, CV_RGB2GRAY);
        return dest;
    }

    static Mat* bgr2gray(Mat* image) {
        Mat* dest = new Mat(*image);
        cv::cvtColor(*image, *dest, CV_BGR2GRAY);
        return dest;
    }

    static vector<Mat> pyramid(Mat* image, Size size, int maxLevel, bool withDerivatives) {
        vector<Mat> pyramid;
        cv::buildOpticalFlowPyramid(*image,
            pyramid,
            size,
            maxLevel,
            withDerivatives
        );
        return pyramid;
    }

    static Mat* gaussian(Mat* image, double sigma) {
        Mat kernel = cv::getGaussianKernel(12, sigma);
        Mat kernelTranspose = Mat(kernel.cols, kernel.rows, kernel.type());
        cv::transpose(kernel, kernelTranspose);
        Mat kernel2d = kernel * kernelTranspose;

        Mat* dest = new Mat(*image);
        cv::filter2D(*image, *dest, -1, kernel2d, Point(-1, -1), 0, BORDER_CONSTANT);
        return dest;
    }
};
#endif
