#ifndef PATCH_H
#define PATCH_H

#include <stdlib.h>

#include "core/Frame.hpp"
#include "core/Box.hpp"

class Patch {
public:
    static int WODTH;
    static int HEIGHT;

    Patch();
    Patch(int* data);
    Patch(Frame* frame, Box* box);
    int* data;
};
#endif
