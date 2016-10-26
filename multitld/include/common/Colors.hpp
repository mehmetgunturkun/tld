#ifndef COLORS_H
#define COLORS_H

#include "opencv2/imgproc/imgproc.hpp"

#define COLOR_BOLD    "\x1b[1m"

#define COLOR_GREEN   "\x1b[92m"
#define COLOR_RED     "\x1b[91m"
#define COLOR_YELLOW  "\x1b[93m"
#define COLOR_CYAN    "\x1b[96m"
#define COLOR_WHITE   "\x1b[37m"

#define COLOR_RESET   "\x1b[0m"

#define CVT_COLOR(STR, ANSII_COLOR_CODE) ANSII_COLOR_CODE STR COLOR_RESET

#define GREEN(str) CVT_COLOR(str, COLOR_GREEN)
#define RED(str) CVT_COLOR(str, COLOR_RED)
#define YELLOW(str) CVT_COLOR(str, COLOR_YELLOW)
#define CYAN(str) CVT_COLOR(str, COLOR_CYAN)
#define WHITE(str) CVT_COLOR(str, COLOR_WHITE)

#define BOLD(str) CVT_COLOR(str, COLOR_BOLD)

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
