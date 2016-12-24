#include "testbase/Sequence.hpp"
#include "common/Colors.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <cmath>

#include <iostream>
#include <string>
#include <sstream>

#include <vector>

using namespace std;

class BoxIteratorScale {
public:
    double width;
    double height;

    double stepX;
    double stepY;

    double limitX;
    double limitY;

    double x;
    double y;

    double i;
    double j;

    bool cursorInitiated;
    int nrOfBoxes;

    BoxIteratorScale(double frameWidth, double frameHeight, double width, double height) {
        this->cursorInitiated = false;

        this->width = width;
        this->height = height;

        this->stepX = fmin(width, height) * 0.1;
        this->stepY = fmin(height, height) * 0.1;

        this->limitX = frameWidth - width - 2.0;
        this->limitY = frameHeight - height - 2.0;

        this->x = 1.0;
        this->y = 1.0;

        this->i = 1.0;
        this->j = 1.0;

        this->nrOfBoxes = 0;
    }

    int lessThanOrEqual(double a, double b) {
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

    vector<Box*> iterate() {
        vector<Box*> boxListPerScale;
        for (double i = 1.0; lessThanOrEqual(i, limitY); i = i + stepY) {
            for (double j = 1.0; lessThanOrEqual(j, limitX); j = j + stepX) {
                double x1 = round(j);
                double y1 = round(i);
                double x2 = x1 + width - 1.0;
                double y2 = y1 + height - 1.0;

                Box* b = new Box(x1, y1, x2, y2);
                printf("%s\n", b->toCharArr());
                boxListPerScale.push_back(b);
            }
        }
        return boxListPerScale;
    }

    bool hasNext() {
        if (nrOfBoxes == 0) {
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


            // double nextI = i + stepY;
            // bool hasNextVertically = lessThanOrEqual(nextI, limitY);
            // if (!hasNextVertically) {
            //     printf("Y >>> %5.4f >= %5.4f\n", nextI, limitY);
            //     return false;
            // }
            //
            // double nextJ = j + stepX;
            // bool hasNextHorizontally = lessThanOrEqual(nextJ, limitX);
            // if (!hasNextHorizontally) {
            //     printf("X >>> %5.4f >= %5.4f\n", nextJ, limitX);
            //     j = 1.0;
            //     i = i + stepY;
            //     return true;
            // } else {
            //     j = j + stepX;
            //     return true;
            // }
        }
    }

    Box* next() {
        double x1 = round(j);
        double y1 = round(i);
        double x2 = x1 + width - 1.0;
        double y2 = y1 + height - 1.0;

        Box* b = new Box(x1, y1, x2, y2);
        return b;
    }
};

std::ostream& operator<<(std::ostream &strm, const BoxIteratorScale* s) {
    return strm << "BoxIteratorScale("
        << s->width << ", "
        << s->height << ", "
        << s->stepX << ", "
        << s->stepY <<  ","
        << s->limitX << ", "
        << s->limitY << ")";
}

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
public:
    int nrOfBoxes;

    void init() {
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
public:
    BoxIterator(Frame* frame, Box* box) {
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

    vector<Box*> iterate() {
        vector<Box*> boxList;
        for (int i = 0; i < 1; i++) {
            BoxIteratorScale* boxScale = scales[i];
            vector<Box*> boxPerScale = (boxScale->iterate());
            boxList.insert(boxList.end(), boxPerScale.begin(), boxPerScale.end());
        }
        return boxList;
    }

    bool hasNext() {
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

    Box* next() {
        BoxIteratorScale* scale = scales[currentScale];
        nrOfBoxes += 1;
        return scale->next();
    }

    void iteratePerScale() {
        BoxIteratorScale* scale = scales[0];
        while (scale->hasNext()) {
            Box* b = scale->next();

            printf("%s\n", b->toCharArr());
        }
    }
};

int main(int argc, char** argv) {
    vector<string> dataset = {
        "01_david",
        "02_jumping",
        "03_pedestrian1",
        "04_pedestrian2",
        "05_pedestrian3",
        "06_car",
        "07_motocross",
        "08_volkswagen",
        "09_carchase",
        "10_panda"
    };

    vector<int> gridSize = {
        26574,
        31231,
        20486,
        12092,
        10789,
        18130,
        31823,
        81963,
        18621,
        31600
    };

    int nrOfDataset = (int) dataset.size();
    printf("There are %d sequences in dataset\n", nrOfDataset);
    for (int i = 0; i < nrOfDataset; i++) {
        string key = dataset[i];
        int size = gridSize[i];

        Sequence* sequence = new Sequence(key);
        Frame* firstFrame = sequence->next();
        Box* firstBox = sequence->initBox;

        BoxIterator* iterator = new BoxIterator(firstFrame, firstBox);
        while (iterator->hasNext()) {
            Box* b = iterator->next();
            free(b);
        }

        int nrOfBoxes = iterator->nrOfBoxes;
        if (nrOfBoxes == size) {
            printf(GREEN("Seq(%s) => total number of boxes: %d = %d\n"), key.c_str(), nrOfBoxes, size);
        } else {
            printf(RED("Seq(%s) => total number of boxes: %d ≠ %d\n"), key.c_str(), nrOfBoxes, size);
        }

        free(iterator);
    }
    return 0;
}
