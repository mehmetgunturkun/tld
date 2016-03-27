#ifndef BOX_HPP
#define BOX_HPP

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string>

#include "common/Option.hpp"
#include "util/Util.h"
#include "util/StringStream.hpp"

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

    Box() {}
    Box(int id, double x1, double y1, double x2, double y2);

    Box* move(float dx, float dy);
    Box* clone();
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
        return 0.0;
    }

    static Box* merge(Box* b0, Box* b1) {
        double x1 = (b0->x1 + b1->x1) / 2;
        double y1 = (b0->y1 + b1->y1) / 2;
        double x2 = (b0->x2 + b1->x2) / 2;
        double y2 = (b0->y2 + b1->y2) / 2;

        Box* meanBox = new Box(0, x1, y1, x2, y2);
        return meanBox;
    }
};

struct OverlapOrdered {
  bool operator() (Box* box1, Box* box2) {
      return box1->overlap >= box2->overlap;
  }
};

#endif
