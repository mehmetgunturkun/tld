#ifndef IMAGE_H
#define IMAGE_H

#define CV_LOAD_IMAGE_GAUSSIAN -1
#include <iostream>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "core/Warp.hpp"
#include "core/Random.hpp"

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
        // cv::buildOpticalFlowPyramid(*image,
        //     pyramid,
        //     size,
        //     maxLevel,
        //     withDerivatives
        // );
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

    static Mat* warp(Mat* img, double x1, double y1, double x2, double y2) {
        // Parameter initialization
        double angle = 20.0;
        double scale = 0.02;
        double shift = 0.02;

        double width = x2 - x1;
        double height = y2 - y1;

        double meanx = (x1 + x2) / 2;
        double meany = (y1 + y2) / 2;

        // Build H
        Mat Sh1 = Mat::zeros(3, 3, CV_64F);
        Sh1.at<double>(0, 0) = 1.0;
        Sh1.at<double>(0, 1) = 0.0;
        Sh1.at<double>(0, 2) = -meanx;

        Sh1.at<double>(1, 0) = 0.0;
        Sh1.at<double>(1, 1) = 1.0;
        Sh1.at<double>(1, 2) = -meany;

        Sh1.at<double>(2, 0) = 0.0;
        Sh1.at<double>(2, 1) = 0.0;
        Sh1.at<double>(2, 2) = 1.0;

        double scaleValue = 1 - scale * (Random::randomFloat() - 0.5);
        Mat Sca = Mat::zeros(3, 3, CV_64F);
        Sca.at<double>(0, 0) = scaleValue;
        Sca.at<double>(1, 1) = scaleValue;
        Sca.at<double>(2, 2) = 1.0;

        double angleValue = 2 * M_PI / 360 * angle * (Random::randomFloat() - 0.5);
        double ca = cos(angleValue);
        double sa = sin(angleValue);

        Mat Ang = Mat::zeros(3, 3, CV_64F);
        Ang.at<double>(0, 0) = ca;
        Ang.at<double>(0, 1) = -sa;
        Ang.at<double>(0, 2) = 0.0;

        Ang.at<double>(1, 0) = sa;
        Ang.at<double>(1, 1) = ca;
        Ang.at<double>(1, 2) = 0.0;

        Ang.at<double>(2, 0) = 0.0;
        Ang.at<double>(2, 1) = 0.0;
        Ang.at<double>(2, 2) = 1.0;

        double shR = shift * (height + 1) * (Random::randomFloat() - 0.5);
        double shC = shift * (width + 1) * (Random::randomFloat() - 0.5);

        Mat Sh2 = Mat::zeros(3, 3, CV_64F);
        Sh2.at<double>(0, 0) = 1.0;
        Sh2.at<double>(0, 1) = 0.0;
        Sh2.at<double>(0, 2) = shC;

        Sh2.at<double>(1, 0) = 0.0;
        Sh2.at<double>(1, 1) = 1.0;
        Sh2.at<double>(1, 2) = shR;

        Sh2.at<double>(2, 0) = 0.0;
        Sh2.at<double>(2, 1) = 0.0;
        Sh2.at<double>(2, 2) = 1.0;

        Mat H = Sh2 * Ang * Sca* Sh1;
        H = H.inv();
        double* h = (double*) H.data;


        double xmin = -(width / 2.0);
        double xmax = (width / 2.0);

        double ymin = -(height / 2.0);
        double ymax = (height / 2.0);

        Mat* warpedImg = Warp::warp(img, h, xmin, ymin, xmax, ymax);
        return warpedImg;
    }

    static Mat* patch(Mat* img, double x1, double y1, double x2, double y2) {
        return img;
    }

    // static Mat* warp(
    //     Mat* img,
    //     double shiftX,
    //     double shiftY,
    //     double angle,
    //     double scale) {
    //         int width = 24;
    //         int height = 24;
    //         Mat* dest = new Mat(img->clone());
    //         Point center = Point(width / 2, height / 2);
    //
    //         double s = 1 - scale * (Random::randomFloat() - 0.5);
    //
    //         double tx = shiftX * width * (Random::randomFloat() - 0.5);
    //         double ty = shiftY * height * (Random::randomFloat() - 0.5);
    //
    //         double a = (double) angle * ( Random::randomFloat() - 0.5);
    //         Mat rotMat = getRotationMatrix2D(center, a, 1.0);
    //         Mat H = Mat::zeros(2, 3, CV_64F);
    //
    //         H.at<double>(0, 0) = s * rotMat.at<double>(0, 0);
    //         H.at<double>(0, 1) = rotMat.at<double>(0, 1);
    //         H.at<double>(0, 2) = tx + rotMat.at<double>(0, 2);
    //
    //         H.at<double>(1, 0) = rotMat.at<double>(1, 0);
    //         H.at<double>(1, 1) = s * rotMat.at<double>(1, 1);
    //         H.at<double>(1, 2) = ty + rotMat.at<double>(1, 2);
    //
    //         cv::warpAffine(*img, *dest, H, dest->size());
    //         return dest;
    // }

    static Mat* warp(Mat* img) {
        return Image::warp(img, 0.02, 0.02, 0.0, 0);
    }
};
#endif
