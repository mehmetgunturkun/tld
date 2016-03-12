#ifndef FB_FLOW_H
#define FB_FLOW_H

#include "common/Option.hpp"

#include "core/ScoredBox.hpp"

#include "tracker/Flow.hpp"
#include "tracker/FBDisplacement.hpp"

class FBFlow {
public:
    bool isValid;
    int displacementSize;
    vector<Displacement*> displacementList;

    static float FB_ERROR_LIMIT;

    FBFlow(Flow* forward, Flow* backward);
    Option<ScoredBox>* estimate(Box* box);
};
#endif
