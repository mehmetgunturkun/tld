#include "core/IO.hpp"

Image* IO::imread(string fileName) {
    Mat img = cv::imread(fileName);
    Image* image = new Image(new Mat(img));
    return image;
}

Image* IO::imread(string fileName, int imageType) {
    Mat img = cv::imread(fileName, imageType);
    Image* image = new Image(new Mat(img));
    return image;
}
