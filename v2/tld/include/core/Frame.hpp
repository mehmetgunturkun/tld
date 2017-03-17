#ifndef FRAME_H
#define FRAME_H

#include <string>

#include "common/Option.hpp"
#include "core/Image.hpp"

class Frame {
public:
    int id;
    string name;

    int width;
    int height;

    IplImage* originalImage;
    IplImage* grayscale;
    IplImage* gaussian;

    Frame(int id, string name, IplImage* originalImage, IplImage* grayscale, IplImage* gaussian);
    ~Frame();

    static Option<Frame*> fromFile(int id, string fileName);
};

#endif
