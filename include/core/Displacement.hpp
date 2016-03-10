#ifndef DISPLACEMENT_H
#define DISPLACEMENT_H

#include "core/Point.hpp"

using namespace std;

class Displacement {
public:
    int pointId;
    tld::Point* source;
    tld::Point* target;

    double dx;
    double dy;

    Displacement(int id, tld::Point* s, tld::Point* t);

};
#endif
