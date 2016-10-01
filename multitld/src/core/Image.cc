#include "core/Image.hpp"

Image::Image(Mat* img) {
    this->underlying = img;
    this->width = img->cols;
    this->height = img->rows;
    this->nrOfChannels = img->channels();
}

Image* Image::rgb2gray(Image* image) {
    Mat* dest = new Mat(*(image->underlying));
    cv::cvtColor(*(image->underlying), *dest, CV_RGB2GRAY);
    Image* destImage = new Image(dest);
    return destImage;
}

Image* Image::bgr2gray(Image* image) {
    Mat* dest = new Mat(*(image->underlying));
    cv::cvtColor(*(image->underlying), *dest, CV_RGB2GRAY);
    Image* destImage = new Image(dest);
    return destImage;
}
