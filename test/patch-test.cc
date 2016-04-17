#include <cstdlib>
#include <cstdio>
#include <ctime>
#include "sys/time.h"
#include "core/Image.hpp"
#include "core/Frame.hpp"
#include "core/Box.hpp"
#include "detector/nn/Patch.hpp"
#include "util/Dataset.hpp"

int main(int argc, char** argv) {
    Dataset* dataset = new Dataset("car");
    Frame* f = dataset->next();

    Box* b = dataset->initBox;
    Patch* sample = new Patch(f, b);

    Image::imshow("patch", sample->data, 0);
    return 0;
}
