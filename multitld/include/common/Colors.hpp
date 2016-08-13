#ifndef COLORS_H
#define COLORS_H

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

class Colors {
public:
    static Scalar RED;
    static Scalar BLUE;
    static Scalar GREEN;
    static Scalar ORANGE;
    static Scalar PURPLE;
    static Scalar YELLOW;

    static vector<Scalar> colors;
};
#endif
