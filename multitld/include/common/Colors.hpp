#ifndef COLORS_H
#define COLORS_H

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

// #define ANSI_COLOR_GREEN   "\x1b[32m"
// #define ANSI_COLOR_RED     "\x1b[31m"
// #define ANSI_COLOR_YELLOW  "\x1b[33m"

#define COLOR_GREEN   "\x1b[32m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_YELLOW  "\x1b[33m"

#define COLOR_RESET   "\x1b[0m"

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
