#include "core/IO.hpp"

int main(int argc, char** args) {
    Image2* jpgImage = IO::imread("resources/test.jpg");
    // printf("Image(JPG) type: %d, #channels: %d\n", jpgImage->type(), jpgImage->channels());
    return 0;
}
