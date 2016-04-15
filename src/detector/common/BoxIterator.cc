#include "detector/common/BoxIterator.hpp"

BoxIterator::BoxIterator(Frame* f, Box* b, int maximumScale, int minimumWindowSize) {
    frame = f;
    boxPointer = b->clone();
    maxScale = maximumScale;
    minWindowSize = minimumWindowSize;
    init();
}

void BoxIterator::init() {
    boxPointer = boxPointer->clone();
    int scaleNo = 0;
    while (boxPointer->width / 1.2 > minWindowSize && boxPointer->height / 1.2 > minWindowSize && (scaleNo < maxScale))  {
        boxPointer->width = boxPointer->width / 1.2;
        boxPointer->height = boxPointer->height / 1.2;
        scaleNo++;
    }
    boxPointer->x1 = 2.0;
    boxPointer->y1 = 2.0;

    boxPointer->x2 = boxPointer->width + boxPointer->x1 - 1;
    boxPointer->y2 = boxPointer->height + boxPointer->y1 - 1;

    boxCount = 0;
    levelBasedCount = 0;

    resetLimitsAndSteps();
}

void BoxIterator::resetLimitsAndSteps() {
    stepX = round(boxPointer->width) * 0.1;
    stepY = round(boxPointer->height) * 0.1;
    stepX = min(stepX, stepY);

    limitX = ((double) frame->width) - round(boxPointer->width) - 1.0;
    limitY = ((double) frame->height) - round(boxPointer->height) - 1.0;
}

bool BoxIterator::hasNext() {
    bool boxSmallerFrame = !(boxPointer->width >= frame->width || boxPointer->height >= frame->height);
    return boxSmallerFrame && maxScale > 0;
}

void BoxIterator::scaleUp() {
    maxScale = maxScale - 1;

    boxPointer->width = boxPointer->width * 1.2;
    boxPointer->height = boxPointer->height * 1.2;

    boxPointer->x1 = 2.0;
    boxPointer->x2 = boxPointer->x1 + boxPointer->width - 1;

    boxPointer->y1 = 2.0;
    boxPointer->y2 = boxPointer->y1 + boxPointer->height - 1;

    resetLimitsAndSteps();
    levelBasedCount = 0;
}

Box* BoxIterator::next() {
    Box* box = Box::rounded(boxPointer);
    box->id = boxCount;

    boxCount++;
    levelBasedCount++;

    double xPointer = boxPointer->x1 + stepX;
    if (xPointer > limitX) {
        boxPointer->x1 = 2.0;
        boxPointer->x2 = boxPointer->width + boxPointer->x1 - 1;

        float yPointer = boxPointer->y1 + stepY;
        if (yPointer > limitY) {
            scaleUp();
        } else {
            boxPointer->y1 = yPointer;
            boxPointer->y2 = yPointer + boxPointer->height - 1;
        }
    } else {
        boxPointer->x1 = xPointer;
        boxPointer->x2 = xPointer + boxPointer->width - 1;
    }
    return box;
}
