#ifndef IMAGE_BUILDER_H
#define IMAGE_BUILDER_H

#include "opencv2/imgproc/imgproc.hpp"

#include "common/Colors.hpp"
#include "core/Frame.hpp"
#include "core/Box.hpp"

class ImageBuilder {
public:
    Mat img;
    string title;

    ImageBuilder();
    ImageBuilder(Mat img);
    ImageBuilder(Frame* frame);
    ImageBuilder withFrame(Frame* frame);
    ImageBuilder withBox(Box* box, Scalar color = Colors::RED);
    ImageBuilder withCircle(Point2f* center, int radius);
    ImageBuilder withTitle(string title);
    void display(int delay = 0);
};
#endif
