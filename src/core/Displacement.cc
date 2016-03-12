#include "core/Displacement.hpp"

Displacement::Displacement(int id,
    tld::Point* s,
    tld::Point* t,
    uchar st,
    float err) {
        pointId = id;
        source = s;
        target = t;
        state = st;
        error = err;

        dx = target->underlying.x - source->underlying.x;
        dy = target->underlying.y - source->underlying.y;
    }
