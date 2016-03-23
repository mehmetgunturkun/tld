#ifndef PATCH_H
#define PATCH_H

#include <stdlib.h>

#include "core/Frame.hpp"
#include "core/Box.hpp"

class Patch {
public:
    static int WIDTH;
    static int HEIGHT;

    Patch(Mat* data);
    Patch(Frame* frame, Box* box);
    Mat* data;
};
#endif
