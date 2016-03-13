#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string.h>
#include <iostream>
#include <fstream>

#include "core/Frame.hpp"

#include "tld/TLD.hpp"

int main(int argc, char** argv) {
    Frame* frame = new Frame("resources/test.jpg");
    Image::imshow("colored", frame->colored);
    Image::imshow("gray", frame->grayscale);
    Image::imshow("gaussian", frame->gaussian);
    return 0;
}
