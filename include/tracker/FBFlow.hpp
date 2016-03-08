#ifndef FB_FLOW_H
#define FB_FLOW_H

#include "common/Option.hpp"

#include "core/ScoredBox.hpp"

#include "tracker/Flow.hpp"

class FBFlow {
public:
    FBFlow(Flow* forward, Flow* backward);
    Option<ScoredBox>* estimate(Box* box);
};
#endif
