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

        // integral = new IntegralImage(grayscale);

        // Mat* grayscaleForGaussian = new Mat(grayscale->clone());
        // gaussian = Image::gaussian(grayscaleForGaussian, 2.0);
        // free(grayscaleForGaussian);

        flowPyramid = Image::pyramid(grayscale, Size(4, 4), 2, true);
    } else {
        grayscale = Image::imread(fileName, CV_LOAD_IMAGE_GRAYSCALE);

        // integral = new IntegralImage(grayscale);

        // Mat* grayscaleForGaussian = Image::bgr2gray(colored);
        // gaussian = Image::gaussian(grayscaleForGaussian, 2.0);
        // free(grayscaleForGaussian);

        flowPyramid = Image::pyramid(grayscale, Size(4, 4), 2, true);
    }
}

void Frame::show() {
    Image::imshow("Frame", this->displayImg, 1000);
}
