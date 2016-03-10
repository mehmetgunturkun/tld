#include "core/Displacement.hpp"

Displacement::Displacement(int id,
    tld::Point* s,
    tld::Point* t) {
        pointId = id;
        source = s;
        target = t;

        dx = target->underlying.x - source->underlying.x;
        dy = target->underlying.y - source->underlying.y;
    }
