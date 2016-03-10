#ifndef FLOW_H
#define FLOW_H

#include <unordered_map>
#include "core/Frame.hpp"
#include "core/Point.hpp"
#include "core/Displacement.hpp"

using namespace std;
using namespace tld;

class Flow {
public:
    Frame* source;
    Frame* target;

    vector<tld::Point*> oldPoints;
    vector<tld::Point*> newPoints;

    int pointCount;

    unordered_map<int, Displacement*> displacementMap;

    Flow(Frame* s,
         Frame* t,
         vector<tld::Point*> op,
         vector<tld::Point*> np);
};
#endif
