#include "util/ImageBuilder.hpp"

ImageBuilder::ImageBuilder() {
    this->title = "Default";
}

ImageBuilder::ImageBuilder(Mat* img) {
    this->title = "Default";
    this->img = img;
}

ImageBuilder::ImageBuilder(Frame* frame) {
    this->title = "Default";
    this->img = new Mat(frame->displayImg->clone());
}

ImageBuilder* ImageBuilder::withFrame(Frame* frame) {
    this->img = new Mat(frame->displayImg->clone());
    return this;
}

ImageBuilder* ImageBuilder::withBox(Box* box, Scalar color) {
    cv::rectangle(
        *img,
        Point2i(box->x1, box->y1),
        Point2i(box->x2 ,box->y2),
        color, 1, 8, 0
    );
    return this;
}

ImageBuilder* ImageBuilder::withTitle(string title) {
    this->title = title;
    return this;
}

void ImageBuilder::display(int delay) {
    cv::imshow(this->title, *(this->img));
    cv::waitKey(delay);
}
