#include "core/Image.hpp"

IplImage* Image::imread(string fileName) {
    return Image::imread(fileName.c_str());
}

IplImage* Image::imread(const char* fileName) {
    IplImage* image = cvLoadImage(fileName);
    return image;
}

IplImage* Image::imread(string fileName, int imageType) {
    return Image::imread(fileName.c_str(), imageType);
}

IplImage* Image::imread(const char* fileName, int imageType) {
    IplImage* image = cvLoadImage(fileName, imageType);
    return image;
}

IplImage* Image::rgb2gray(IplImage* image) {
    IplImage* dest = cvCreateImage(cvGetSize(image), image->depth, 1);
    cvCvtColor(image, dest, CV_RGB2GRAY);
    return dest;
}

IplImage* Image::bgr2gray(IplImage* image) {
    IplImage* dest = cvCreateImage(cvGetSize(image), image->depth, 1);
    cvCvtColor(image, dest, CV_BGR2GRAY);
    return dest;
}

IplImage* Image::warp(IplImage* img,
    double x1,
    double y1,
    double x2,
    double y2,
    double angle,
    double scale,
    double shift) {
        // Parameter initialization
        // double angle = 20.0;
        // double scale = 0.02;
        // double shift = 0.02;

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

        IplImage* warpedImg = Warp::warp(img, h, xmin, ymin, xmax, ymax);
        return warpedImg;
}
