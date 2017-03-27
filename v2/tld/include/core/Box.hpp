#ifndef BOX_H
#define BOX_H

#include <string>
#include <sstream>
#include <cmath>

#include "common/Option.hpp"
#include "common/string/StringStream.hpp"

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

    //For tracking. TODO Find a better way to do that.
    int nrOfPoints;

    Box(double x1, double y1, double x2, double y2);
    Box(int id, double x1, double y1, double x2, double y2);

    Box* move(double dx, double sx, double dy, double sy);

    string toString();
    string toTLDString();
    const char* toCharArr();

    static Option<Box*> fromLine(string line);
    static double computeOverlap(Box* b1, Box* b2);
};

struct OverlapOrdered {
  bool operator() (Box* box1, Box* box2) {
      return box1->overlap >= box2->overlap;
  }

  bool compare(Box* box1, Box* box2) {
      return box1->overlap > box2->overlap;
  }
};

#endif
