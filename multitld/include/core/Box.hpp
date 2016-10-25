#ifndef BOX_H
#define BOX_H

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string>
#include <vector>

#include "common/Option.hpp"
#include "common/StringStream.hpp"

#include <unordered_set>

#define NR_OF_MODELS 2
#define MIN_DOUBLE(a,b) (a < b ? a : b)
#define MAX_DOUBLE(a,b) (a > b ? a : b)

using namespace std;

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

    int nrOfPoints;
    int scaleNo;
    bool isValid;

    Box(int id, double x1, double y1, double x2, double y2);
    Box* move(double dx, double dy);

    Box* sum(Box* other);
    Box* divide(int n);

    Box* clone();
    vector<Box*> splitTwo();

    static Option<Box>* parseFromLine(string line) {
        StringStream* stream = new StringStream(line, ',');

        double x1 = stod(stream->next());
        double y1 = stod(stream->next());
        double x2 = stod(stream->next());
        double y2 = stod(stream->next());

        if (isnan(x1) || isnan(x2) || isnan(y1) || isnan(y2)) {
            Option<Box>* none = new Option<Box>();
            return none;
        }

        Box* box = new Box(0, x1 - 1, y1 - 1, x2 - 1, y2 - 1);
        Option<Box>* maybeBox = new Option<Box>(box);
        return maybeBox;
    }

    string toString() {
        stringstream ss;
        ss  << "Box("
            << id << ", "
            << x1 << ", "
            << y1 << ", "
            << x2 << ", "
            << y2 << ", "
            << width << ", "
            << height<< ")";
        return ss.str();
    }

    string toTLDString() {
        stringstream ss;
        ss  << "Box("
            << id << ", "
            << x1 << ", "
            << y1 << ", "
            << x2 << ", "
            << y2 << ", "
            << width << ", "
            << height<< ")";
        return ss.str();
    }

    const char* toCharArr() {
        string str = this->toString();
        return str.c_str();
    }

    static Box* rounded(Box* box) {
        Box* newBox = new Box(
            box->id,
            round(box->x1),
            round(box->y1),
            round(box->x1) + round(box->width) - 1.0,
            round(box->y1) + round(box->height) - 1.0
        );
        return newBox;
    }

    static double computeOverlap(Box* b1, Box* b2) {
        double x11 = b1->x1;
        double x12 = b1->x2;
        double y11 = b1->y1;
        double y12 = b1->y2;
        double x21 = b2->x1;
        double x22 = b2->x2;
        double y21 = b2->y1;
        double y22 = b2->y2;

        if (x11 > x22 ||
            y11 > y22 ||
            x12 < x21 ||
            y12 < y21) {
                return 0;
        }

        double intersectionX = MIN_DOUBLE(x12, x22) - MAX_DOUBLE(x11, x21) + 1;
        double intersectionY = MIN_DOUBLE(y12, y22) - MAX_DOUBLE(y11, y21) + 1;
        double intersection = intersectionX * intersectionY;

        double area1 = (x12 - x11 + 1) * (y12 - y11 + 1);
        double area2 = (x22 - x21 + 1) * (y22 - y21 + 1);
        double overlap = intersection / (area1 + area2 - intersection);

        if (overlap > 1.0) {
            printf("Compare:\n");
            printf("\t%s:\n", b1->toCharArr());
            printf("\t%s:\n", b2->toCharArr());

            printf("MIN(%g, %g) = %g\n", x12, x22, MIN_DOUBLE(x12, x22));
            printf("MAX(%g, %g) = %g\n", x11, x21, MAX_DOUBLE(x11, x21));
            printf("MIN(%g, %g) = %g\n", y12, y22, MIN_DOUBLE(y12, y22));
            printf("MAX(%g, %g) = %g\n", y11, y21, MAX_DOUBLE(y11, y21));

            printf("INTx: %g\n", intersectionX);
            printf("INTy: %g\n", intersectionY);
            printf("INTxy: %g\n", intersection);
            printf("AREA1: %g\n", area1);
            printf("AREA2: %g\n", area2);
        }

        return overlap;
    }

    static vector<double> computeOverlap(Box* b1, vector<Box*> boxList, int nrOfBoxes) {
        vector<double> overlaps(nrOfBoxes);
        for (int i = 0; i < nrOfBoxes; i++) {
            Box* b2 = boxList[i];
            overlaps[i] = Box::computeOverlap(b1, b2);
        }
        return overlaps;
    }

    static Option<Box>* None;

};

struct OverlapOrdered {
  bool operator() (Box* box1, Box* box2) {
      return box1->overlap >= box2->overlap;
  }
};

#endif
