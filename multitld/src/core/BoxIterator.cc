#include "core/BoxIterator.hpp"

double roundDouble(double n) {
    int intN = (int) n;
    double precision = n - intN;
    if (precision >= 0.49) {
        return (double) intN + 1;
    } else {
        return (double) intN;
    }
}

BoxScaleTemplate::BoxScaleTemplate(double boxWidth, double boxHeight, double frameWidth, double frameHeight) {
    this->x1 = 1.0;
    this->y1 = 1.0;

    this->preStart = true;
    this->nrOfBoxes = 0;

    this->width = boxWidth;
    this->height = boxHeight;

    this->stepX = round(this->width) * 0.1;
    this->stepY = round(this->height) * 0.1;

    this->stepX = min(this->stepX, this->stepY);
    this->stepY = min(this->stepX, this->stepY);

    this->limitX = roundDouble(frameWidth - this->width - 1);
    this->limitY = roundDouble(frameHeight - this->height - 1);
}


bool BoxScaleTemplate::hasNext() {
    if (this->preStart) {
        this->preStart = false;
        return true;
    } else {
        double nextX1 = this->x1 + stepX;
        if (roundDouble(nextX1) >= limitX) {
            double nextY1 = this->y1 + stepY;
            if (roundDouble(nextY1) >= limitY) {
                return false;
            }
            this->x1 = 1.0;
            this->y1 = nextY1;
        } else {
            this->x1 = nextX1;
        }
        return true;
    }
}

Box* BoxScaleTemplate::next() {
    nrOfBoxes += 1;
    Box* box = new Box(0,
        roundDouble(this->x1),
        roundDouble(this->y1),
        roundDouble(this->x1) + this->width - 1,
        roundDouble(this->y1) + this->height - 1
    );
    return box;
}

BoxIterator::BoxIterator(Frame* f, Box* b, int maximumScale, int minimumWindowSize) {
    this->currentScaleNo = 0;
    this->maxScale = 10;
    this->nrOfTotalScales = 0;
    this->nrOfBoxes = 0;

    initBoxScaleTemplates(f, b);
}

void BoxIterator::initBoxScaleTemplates(Frame* frame, Box* box) {
    double width = (double) box->width;
    double height = (double) box->height;

    double scaleFactor = 1.2;
    for (int i = -10; i <= 10; i++) {
        double scale = pow(scaleFactor, i);
        double scaledWidth = roundDouble(((double) width) * scale);
        double scaledHeight = roundDouble(((double) height) * scale);
        if (scaledWidth > 24.0 && scaledHeight > 24.0  && scaledWidth < frame->width && scaledHeight < frame->height) {
            this->nrOfTotalScales += 1;
            BoxScaleTemplate* boxTemplate = new BoxScaleTemplate(scaledWidth, scaledHeight, frame->width, frame->height);
            boxPerScale.push_back(boxTemplate);
        }
    }
}

bool BoxIterator::hasNext() {
    if (this->currentScaleNo < maxScale && this->currentScaleNo < nrOfTotalScales) {
        BoxScaleTemplate* boxTemplate = boxPerScale[this->currentScaleNo];
        bool hasMoreTemplate = boxTemplate->hasNext();
        if (hasMoreTemplate) {
            return true;
        } else {
            this->currentScaleNo += 1;
            return hasNext();
        }
    } else {
        return false;
    }
}

Box* BoxIterator::next() {
    BoxScaleTemplate* boxTemplate = boxPerScale[this->currentScaleNo];
    Box* b = boxTemplate->next();
    this->nrOfBoxes += 1;
    b->id = this->nrOfBoxes;
    return b;
}
