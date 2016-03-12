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

    int displacementCount;
    vector<Displacement*> displacementList;

    //TODO We need to set those
    vector<tld::Point*> oldPoints;
    vector<tld::Point*> newPoints;

    Flow(Frame* s,
         Frame* t,
         vector<tld::Point*> op,
         vector<tld::Point*> np,
         vector<uchar> status,
         vector<float> errors
     );
};
#endif
