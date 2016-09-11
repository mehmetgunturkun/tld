#include "core/Image.hpp"

int main(int argc, char** args) {
    // ------------------------------------------------------------------------------------------
    Mat* jpgImage = Image::imread("resources/test.jpg");
    printf("Image(JPG) type: %d, #channels: %d\n", jpgImage->type(), jpgImage->channels());
    Image::imshow("jpg - 0", jpgImage, 5000);

    Mat* warpedImage = Image::warp(jpgImage);
    Image::imshow("jpg - 0", warpedImage, 5000);
}
