#include "core/BoxIterator.hpp"

BoxIteratorScale::BoxIteratorScale(double frameWidth, double frameHeight, double width, double height) {
    this->width = width;
    this->height = height;

    this->stepX = fmin(width, height) * 0.1;
    this->stepY = fmin(height, height) * 0.1;

    this->limitX = frameWidth - width - 2.0;
    this->limitY = frameHeight - height - 2.0;

    this->i = 1.0;
    this->j = 1.0;

    this->nrOfBoxes = 0;
}

bool BoxIteratorScale::lessThanOrEqual(double a, double b) {
    double diff = std::abs(a - b);
    if (diff < 0.001) {
        return true;
    } else {
        diff = a - b;
        if (diff > 0.0) {
            return false;
        } else {
            return true;
        }
    }
}

bool BoxIteratorScale::hasNext() {
    if (nrOfBoxes == 0) {
        // Cursor is not initiated yet!
        nrOfBoxes += 1;
        return true;
    } else {
        nrOfBoxes += 1;

        double nextJ = j + stepX;
        bool hasNextHorizontally = lessThanOrEqual(nextJ, limitX);

        if (!hasNextHorizontally) {
            j = 1.0;
            i = i + stepY;
        } else {
            j = j + stepX;
        }

        bool hasNextVertically = lessThanOrEqual(i, limitY);
        if (!hasNextVertically) {
            return false;
        } else {
            return true;
        }
    }
}

Box* BoxIteratorScale::next() {
    double x1 = round(j);
    double y1 = round(i);
    double x2 = x1 + width - 1.0;
    double y2 = y1 + height - 1.0;

    Box* b = new Box(x1, y1, x2, y2);
    return b;
}

BoxIterator::BoxIterator(Frame* frame, Box* box) {
    this->scaleFactor = 1.2;
    this->width = box->width;
    this->height = box->height;

    this->frameWidth = frame->width;
    this->frameHeight = frame->height;

    this->minWin = 24;

    if (box->width < minWin) {
        this->minWin = box->width;
    }

    if (box->height < minWin) {
        this->minWin = box->height;
    }

    this->init();
}

void BoxIterator::init() {
    currentScale = 0;
    totalScale = 0;
    nrOfBoxes = 0;

    for (int i = -10; i <= 10; i++) {
        double scale = pow(scaleFactor, i);
        double scaledWidth = round(this->width * scale);
        double scaledHeight = round(this->height * scale);

        if (scaledWidth >= minWin && scaledHeight >= minWin && scaledWidth < frameWidth && scaledHeight < frameHeight) {
            BoxIteratorScale* boxScale = new BoxIteratorScale(frameWidth, frameHeight, scaledWidth, scaledHeight);
            totalScale += 1;
            scales.push_back(boxScale);
        }
    }
}

bool BoxIterator::hasNext() {
    if (currentScale >= totalScale) {
        return false;
    }

    BoxIteratorScale* scale = scales[currentScale];
    bool hasNextForScale = scale->hasNext();
    if (hasNextForScale) {
        return true;
    } else {
        currentScale += 1;
        return this->hasNext();
    }
}

Box* BoxIterator::next() {
    BoxIteratorScale* scale = scales[currentScale];
    nrOfBoxes += 1;
    return scale->next();
}
