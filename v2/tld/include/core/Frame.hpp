#ifndef FRAME_H
#define FRAME_H

#include <string>

#include "common/Option.hpp"
#include "core/Image.hpp"
#include "core/IntegralImage.hpp"
#include "core/Box.hpp"

#include "core/Gaussian.hpp"

class Frame {
public:
    int id;
    string name;

    int width;
    int height;

    IplImage* originalImage;
    IplImage* grayscale;
    IplImage* gaussian;

    IntegralImage* integralImage;

    Frame(int id, string name, IplImage* originalImage, IplImage* grayscale, IplImage* gaussian);
    ~Frame();

    int get(Point2f* point);
    Frame* clone();

    static Frame* warp(Frame* frame, Box* boxHull);
    static Option<Frame*> fromFile(int id, string fileName);
};

#endif
