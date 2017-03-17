#ifndef BOX_H
#define BOX_H

#include <string>
#include <sstream>
#include <cmath>

#include "common/Option.hpp"
#include "common/string/StringStream.hpp"

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

    Box(double x1, double y1, double x2, double y2);

    string toString();
    string toTLDString();
    const char* toCharArr();

    static Option<Box*> fromLine(string line);
};
#endif
