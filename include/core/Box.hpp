#ifndef BOX_HPP
#define BOX_HPP

class Box {

public:
    int id;
    double x1;
    double y1;
    double x2;
    double y2;

    double width;
    double height;

    double mean;
    double variance;
    double overlap;

    Box();
    Box(int id, double x1, double y1, double x2, double y2);

    static double computeOverlap(Box* b1, Box* b2) {
        return 0.0;
    }
};
#endif
