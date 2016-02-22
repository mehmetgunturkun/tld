#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string.h>
#include <iostream>
#include <fstream>

#include "core/Image.hpp"

int main(int argc, char** argv) {

    Mat* testImage = Image::imread("resources/test.pgm");
    Image::imshow("test", testImage);

    return 0;
}
