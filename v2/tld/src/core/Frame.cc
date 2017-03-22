#include "core/Frame.hpp"

Frame::Frame(int id, string name, IplImage* originalImage, IplImage* grayscale, IplImage* gaussian) {
    this->id = id;
    this->name = name;

    this->originalImage = originalImage;
    this->grayscale = grayscale;
    this->gaussian = gaussian;

    this->integralImage = new IntegralImage(this->grayscale);

    this->width = originalImage->width;
    this->height = originalImage->height;
}

Frame::~Frame() {
    cvReleaseImage(&originalImage);
    cvReleaseImage(&grayscale);
    delete integralImage;
}


Option<Frame*> Frame::fromFile(int id, string fileName) {
    IplImage* originalImage = Image::imread(fileName, CV_LOAD_IMAGE_ANYCOLOR);
    IplImage* grayImage = Image::imread(fileName, CV_LOAD_IMAGE_GRAYSCALE);

    Frame* frame = new Frame(id, fileName, originalImage, grayImage, grayImage);

    Option<Frame*> maybeFrame = Option<Frame*>(frame);
    return maybeFrame;
}
