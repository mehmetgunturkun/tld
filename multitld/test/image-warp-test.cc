#include "core/Image.hpp"

int main(int argc, char** args) {
    // ------------------------------------------------------------------------------------------
    Mat* jpgImage = Image::imread("resources/test.jpg");
    printf("Image(JPG) type: %d, #channels: %d\n", jpgImage->type(), jpgImage->channels());

    Mat* warpedImage = Image::warp(jpgImage);
}
