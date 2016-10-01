#ifndef IO_H
#define IO_H

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "core/Image2.hpp"

using namespace cv;

class IO {
public:
    static Image2* imread(string fileName);
};
#endif
