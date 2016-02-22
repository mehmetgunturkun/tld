#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/tracking.hpp"

using namespace std;
using namespace cv;

#define CV_LOAD_IMAGE_GAUSSIAN -1

class Image {
public:
    static Mat* imread(string fileName, int imageType = CV_LOAD_IMAGE_ANYCOLOR) {
        Mat tmpImage = cv::imread(fileName, imageType);
        Mat* image = new Mat(tmpImage);
        return image;
    }

    static void imwrite(string fileName, Mat* image) {
        cv::imwrite(fileName, *image);
    }

    static void imshow(string title, Mat* image, int delay = 0) {
        cv::imshow(title, *image);
        cv::waitKey(0);
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

    static Mat* gaussian(Mat* image, double sigma) {
        Mat kernel = cv::getGaussianKernel(12, sigma);
        Mat kernelTranspose = Mat(kernel.cols, kernel.rows, kernel.type());
        cv::transpose(kernel, kernelTranspose);
        Mat kernel2d = kernel * kernelTranspose;

        Mat* dest = new Mat(*image);
        cv::filter2D(*image, *dest, -1, kernel2d, Point(-1, -1), 0, BORDER_CONSTANT);
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
};
#endif
