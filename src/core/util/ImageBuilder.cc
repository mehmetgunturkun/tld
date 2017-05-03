#include "core/util/ImageBuilder.hpp"

ImageBuilder::ImageBuilder() {
    this->title = "Default";
}

ImageBuilder::ImageBuilder(Mat img) {
    this->title = "Default";
    this->img = img;
}

ImageBuilder::ImageBuilder(Frame* frame) {
    this->title = "Default";

    IplImage* dest = cvCreateImage(cvGetSize(frame->originalImage), frame->originalImage->depth, 3);
    cvCvtColor(frame->originalImage, dest, CV_GRAY2RGB);
    this->img = cvarrToMat(dest);
}

ImageBuilder ImageBuilder::withFrame(Frame* frame) {
    IplImage* dest = cvCreateImage(cvGetSize(frame->originalImage), frame->originalImage->depth, 3);
    cvCvtColor(frame->originalImage, dest, CV_GRAY2RGB);
    this->img = cvarrToMat(dest);
    return *this;
}

ImageBuilder ImageBuilder::withBox(Box* box, Scalar color) {
    cv::rectangle(
        img,
        Point2i(box->x1, box->y1),
        Point2i(box->x2 ,box->y2),
        color, 1, 8, 0
    );
    return *this;
}

ImageBuilder ImageBuilder::withTitle(string title) {
    this->title = title;
    return *this;
}

ImageBuilder ImageBuilder::withCircle(Point2f* center, int radius) {
    cv::circle(img, *center, radius, Colors::RED);
    return *this;
}


void ImageBuilder::display(int delay) {
    cv::imshow(this->title, this->img);
    cv::waitKey(delay);
}
