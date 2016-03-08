#ifndef DISPLACEMENT_H
#define DISPLACEMENT_H

#include "core/Point.hpp"

using namespace std;

class Displacement {
public:
    tld::Point* source;
    tld::Point* target;

    double dx;
    double dy;

};
#endif
