#include "core/Frame.hpp"

Frame::Frame(string fileName) {
    name = fileName;
    colored = Image::imread(fileName, CV_LOAD_IMAGE_ANYCOLOR);
    int nrOfChannels = colored->channels();
    if (nrOfChannels == 1) {
        grayscale = Image::imread(fileName, CV_LOAD_IMAGE_GRAYSCALE);

        integral = new IntegralImage(grayscale);

        Mat* grayscaleForGaussian = new Mat(grayscale->clone());
        gaussian = Image::gaussian(grayscaleForGaussian, 2.0);
        free(grayscaleForGaussian);

        flowPyramid = Image::pyramid(grayscale, Size(4, 4), 2, true);
    } else {
        grayscale = Image::imread(fileName, CV_LOAD_IMAGE_GRAYSCALE);

        integral = new IntegralImage(grayscale);

        Mat* grayscaleForGaussian = Image::bgr2gray(colored);
        gaussian = Image::gaussian(grayscaleForGaussian, 2.0);
        free(grayscaleForGaussian);

        flowPyramid = Image::pyramid(grayscale, Size(4, 4), 2, true);
    }
}

int Frame::get(Point2f* point, int imageType) {
    int pixel = (int) gaussian->at<uchar>((int) point->y, (int) point->x);
    return pixel;
}

int Frame::get(int x, int y, int imageType) {
    int pixel = (int) gaussian->at<uchar>(y, x);
    return pixel;
}
