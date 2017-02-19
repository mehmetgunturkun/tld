#include "core/Frame.hpp"

Frame::Frame(string fileName) {
    name = fileName;
    Mat* colored = Image::imread(fileName, CV_LOAD_IMAGE_ANYCOLOR);
    this->displayImg = colored;
    width = colored->cols;
    height = colored->rows;
    int nrOfChannels = colored->channels();
    if (nrOfChannels == 1) {
        grayscale = Image::imread(fileName, CV_LOAD_IMAGE_GRAYSCALE);

        integral = new IntegralImage(grayscale);

        Mat* grayscaleForGaussian = new Mat(grayscale->clone());
        gaussian = Gaussian::blur(grayscaleForGaussian, 2.0);
        free(grayscaleForGaussian);

        flowPyramid = Image::pyramid(grayscale, Size(4, 4), 2, true);
    } else {
        grayscale = Image::imread(fileName, CV_LOAD_IMAGE_GRAYSCALE);

        integral = new IntegralImage(grayscale);

        Mat* grayscaleForGaussian = Image::bgr2gray(colored);
        gaussian = Gaussian::blur(grayscaleForGaussian, 2.0);
        free(grayscaleForGaussian);

        flowPyramid = Image::pyramid(grayscale, Size(4, 4), 2, true);
    }
}

Frame::Frame(Mat* grayscale, Mat* gaussian) {
    this->width = grayscale->cols;
    this->height = grayscale->rows;
    this->grayscale = grayscale;
    this->gaussian = gaussian;
    this->displayImg = grayscale;
}

int Frame::get(Point2f* point, int imageType) {
    int pixel = (int) gaussian->at<uchar>((int) point->y, (int) point->x);
    return pixel;
}

int Frame::get(int x, int y, int imageType) {
    int pixel = (int) gaussian->at<uchar>(y, x);
    return pixel;
}

Frame* Frame::clone() {
    Mat* otherGrayscale = new Mat();
    this->grayscale->copyTo(*otherGrayscale);

    Mat* otherGaussian = new Mat();
    this->gaussian->copyTo(*otherGaussian);

    Frame* other = new Frame(otherGrayscale, otherGaussian);
    return other;
}

Frame* Frame::warp(Frame* frame, Box* box) {
    Frame* copied = frame->clone();
    double x1 = box->x1;
    double y1 = box->y1;

    double x2 = box->x2;
    double y2 = box->y2;
    Mat* gaussian = copied->gaussian;

    Random::seed();
    Mat* warpedImage = Image::warp(gaussian, x1, y1, x2, y2);
    for (int i = 0; i < warpedImage->rows; i++) {
        for (int j = 0; j < warpedImage->cols; j++) {
            double p = warpedImage->at<double>(i, j);
            int pint = (int) round(p);
            gaussian->at<uchar>(i + y1, j + x1) = (uchar) pint;
        }
    }
    return copied;
}

void Frame::dealloc(Frame* frame) {
    free(frame->displayImg);
    free(frame->grayscale);
    free(frame->gaussian);

    IntegralImage::dealloc(frame->integral);
    free(frame);
}
