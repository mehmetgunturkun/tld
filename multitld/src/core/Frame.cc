#include "core/Frame.hpp"

Frame::Frame(string fileName) {
    this->name = fileName;
    this->displayImg = IO::imread(fileName, CV_LOAD_IMAGE_ANYCOLOR);
    this->width = this->displayImg->width;
    this->height = this->displayImg->height;

    int nrOfChannels = this->displayImg->nrOfChannels;
    if (nrOfChannels == 1) {
        grayscale = IO::imread(fileName, CV_LOAD_IMAGE_GRAYSCALE);
        integral = new IntegralImage(grayscale);
        gaussian = new Image(Gaussian::blur(grayscale, 2.0));

        // flowPyramid = Image::pyramid(grayscale, Size(4, 4), 2, true);
    } else {
        grayscale = IO::imread(fileName, CV_LOAD_IMAGE_GRAYSCALE);

        integral = new IntegralImage(grayscale);

        Image* grayscaleForGaussian = Image::bgr2gray(this->displayImg);
        gaussian = new Image(Gaussian::blur(grayscaleForGaussian, 2.0));
        free(grayscaleForGaussian);

        // flowPyramid = Image::pyramid(grayscale, Size(4, 4), 2, true);
    }
}

Frame::Frame(Mat* grayscale, Mat* gaussian) {
    this->width = grayscale->cols;
    this->height = grayscale->rows;
    this->grayscale = new Image(grayscale);
    this->gaussian = new Image(gaussian);
    this->displayImg = new Image(grayscale);
}

int Frame::get(Point2f* point, int imageType) {
    int pixel = (int) gaussian->underlying->at<uchar>((int) point->y, (int) point->x);
    return pixel;
}

int Frame::get(int x, int y, int imageType) {
    int pixel = (int) gaussian->underlying->at<uchar>(y, x);
    return pixel;
}
