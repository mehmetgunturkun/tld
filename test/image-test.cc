#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string.h>
#include <iostream>
#include <fstream>

#include "core/Image.hpp"

int main(int argc, char** argv) {

    Mat* testImage = Image::imread("resources/test.jpg");
    Image::imshow("test.pre", testImage);
    for (int i = 0; i < 20; i++) {
        Mat* warpedImage = Image::warp(testImage, 0.02, 0.02, 10.0, 0.02);
        Image::imshow("warped", warpedImage, 500);
    }
    return 0;
}
