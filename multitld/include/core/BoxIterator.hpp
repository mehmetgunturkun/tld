#ifndef BOX_ITERATOR_H
#define BOX_ITERATOR_H

#include "core/Frame.hpp"
#include "core/Box.hpp"

class BoxIterator {
private:
    Frame* frame;
    Box* boxPointer;

    double limitX;
    double limitY;

    double stepX;
    double stepY;

    int maxScale;
    int minWindowSize;
    int boxCount;
    int levelBasedCount;

    void init();
    void resetLimitsAndSteps();
    void scaleUp();
public:
    BoxIterator(Frame* frame, Box* box, int maximumScale = 10, int minimumWindowSize = 24);
    bool hasNext();
    Box* next();
};
#endif
