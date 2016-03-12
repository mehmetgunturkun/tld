#ifndef FB_DISPLACEMENT_H
#define FB_DISPLACEMENT_H

#include "core/Point.hpp"
#include "core/Displacement.hpp"

using namespace std;

class FBDisplacement {
public:
    Displacement* forward;
    Displacement* backward;

    float fbError;
    float nccError;

    FBDisplacement(Displacement* fwd, Displacement* bwd, float fbe, float ncc);
};
#endif
