#ifndef BOX_ITERATOR_H
#define BOX_ITERATOR_H

#include "core/Frame.hpp"
#include "core/Box.hpp"

class BoxScaleTemplate {
private:
    // double roundDouble(double n);
public:
    int scaleNo;
    double x1;
    double y1;

    double width;
    double height;

    double stepX;
    double stepY;

    double limitX;
    double limitY;

    bool preStart;
    int nrOfBoxes;

    BoxScaleTemplate(int scaleNo, double boxWidth, double boxHeight, double frameWidth, double frameHeight);
    bool hasNext();
    Box* next();
};

class BoxIterator {
private:
    vector<BoxScaleTemplate*> boxPerScale;

    int currentScaleNo;
    int nrOfTotalScales;
    int maxScale;

    int nrOfBoxes;

    void initBoxScaleTemplates(Frame* frame, Box* box);
public:
    BoxIterator(Frame* frame, Box* box, int maximumScale = 10, int minimumWindowSize = 24);
    bool hasNext();
    Box* next();
};
#endif
