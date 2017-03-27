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

    //TODO Check the tru way to release gaussian image.
    delete gaussian;
    delete integralImage;
}

int Frame::get(Point2f* point) {
    //TODO Should use gaussian
    int index = this->width * (int) point->y + (int) point->x;
    int pixel = (int) grayscale->imageData[index];
    return pixel;
}

Frame* Frame::clone() {
    //TODO Implementation
    return this;
}

Option<Frame*> Frame::fromFile(int id, string fileName) {
    IplImage* originalImage = Image::imread(fileName, CV_LOAD_IMAGE_ANYCOLOR);
    IplImage* grayImage = Image::imread(fileName, CV_LOAD_IMAGE_GRAYSCALE);
    IplImage* gaussianImage = Gaussian::blur(grayImage, 12, 2.0);

    Frame* frame = new Frame(id, fileName, originalImage, grayImage, gaussianImage);

    Option<Frame*> maybeFrame = Option<Frame*>(frame);
    return maybeFrame;
}

Frame* Frame::warp(Frame* frame, Box* hull) {
    //TODO Implementation
    return frame;
}
