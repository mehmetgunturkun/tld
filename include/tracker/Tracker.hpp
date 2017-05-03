#ifndef TRACKER_H
#define TRACKER_H

#include "core/Frame.hpp"
#include "core/Box.hpp"

using namespace std;

class Tracker {
public:
    virtual ~Tracker();
    virtual vector<Box*> track(Frame* prev, Frame* curr, vector<Box*> boxList);
};

#endif
