#ifndef IO_H
#define IO_H

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "core/Image.hpp"

using namespace cv;

class IO {
public:
    static Image* imread(string fileName);
    static Image* imread(string fileName, int imageType);
};
#endif
