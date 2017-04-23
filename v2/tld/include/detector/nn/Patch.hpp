#ifndef PATCH_H
#define PATCH_H

#include <stdlib.h>

#include "core/Frame.hpp"
#include "core/Box.hpp"
#include "core/Image.hpp"
#include "core/Warp.hpp"

using namespace std;

class Patch {
public:
    static int WIDTH;
    static int HEIGHT;

    int id;
    IplImage* data;

    Patch(Frame* frame, Box* box);
    ~Patch();
    string toString();

    static double computeVariance(Patch* patch);

};
#endif
