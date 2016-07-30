#ifndef BOX_H
#define BOX_H

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string>

#include "common/Option.hpp"
#include "common/StringStream.hpp"

#include <unordered_set>

using namespace std;

class Box {
public:
    int id;

    float x1;
    float y1;
    float x2;
    float y2;

    float width;
    float height;

    float mean;
    float variance;
    float overlap;

    int nrOfPoints;

    Box(int id, float x1, float y1, float x2, float y2);
    Box* move(float dx, float dy);
    Box* clone();

    static Option<Box>* parseFromLine(string line) {
        StringStream* stream = new StringStream(line, ',');

        float x1 = stod(stream->next());
        float y1 = stod(stream->next());
        float x2 = stod(stream->next());
        float y2 = stod(stream->next());

        if (isnan(x1) || isnan(x2) || isnan(y1) || isnan(y2)) {
            Option<Box>* none = new Option<Box>();
            return none;
        }

        Box* box = new Box(0, x1, y1, x2, y2);
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
            << y2 << ")";
        return ss.str();
    }

    static Box* rounded(Box* box) {
        Box* newBox = new Box(
            box->id,
            round(box->x1),
            round(box->y1),
            round(box->x1) +
            round(box->width) - 1.0,
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

        double intersectionX = min(x12, x22) - max(x11, x21) + 1;
        double intersectionY = min(y12, y22) - max(y11, y21) + 1;
        double intersection = intersectionX * intersectionY;

        double area1 = (x12 - x11 + 1) * (y12 - y11 + 1);
        double area2 = (x22 - x21 + 1) * (y22 - y21 + 1);
        double overlap = intersection / (area1 + area2 - intersection);
        return overlap;
    }
};

struct OverlapOrdered {
  bool operator() (Box* box1, Box* box2) {
      return box1->overlap >= box2->overlap;
  }
};

#endif
