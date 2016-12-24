#ifndef BOX_ITERATOR_H
#define BOX_ITERATOR_H

#include "core/Frame.hpp"
#include "core/Box.hpp"

class BoxIteratorScale {
private:
    double width;
    double height;

    double stepX;
    double stepY;

    double limitX;
    double limitY;

    double i;
    double j;

    int nrOfBoxes;

    bool lessThanOrEqual(double a, double b);
public:
    BoxIteratorScale(double frameWidth, double frameHeight, double width, double height);

    bool hasNext();
    Box* next();
};

class BoxIterator {
private:
    double scaleFactor;
    double width;
    double height;

    double frameWidth;
    double frameHeight;

    double minWin;

    int currentScale;
    int totalScale;


    vector<BoxIteratorScale*> scales;
    void init();

public:
    int nrOfBoxes;
    BoxIterator(Frame* frame, Box* box);

    bool hasNext();
    Box* next();
};
#endif
