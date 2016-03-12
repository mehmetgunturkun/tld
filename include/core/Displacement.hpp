#ifndef DISPLACEMENT_H
#define DISPLACEMENT_H

#include "core/Point.hpp"

using namespace std;

class Displacement {
public:
    int pointId;
    tld::Point* source;
    tld::Point* target;
    uchar state;
    float error;

    double dx;
    double dy;

    Displacement(int id, tld::Point* s, tld::Point* t, uchar st, float err);

};
#endif
