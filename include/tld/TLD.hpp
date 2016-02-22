#ifndef TLD_HPP
#define TLD_HPP

#include "common/Option.hpp"

#include "core/Frame.hpp"
#include "core/Box.hpp"

#include "tracker/Tracker.hpp"
#include "detector/Detector.hpp"

using namespace std;

class TLD {
private:
    Option<Box>* integrate(Option<Box*> maybeCurrentBox, vector<Box*> candidateBoxList);
public:
    Tracker* tracker;
    Detector* detector;

    TLD(Tracker* t, Detector* d);

    void init(Frame* firstFrame, Box* firstBox);
    Option<Box>* process(Frame* prev, Frame* current, Option<Box>* prevBox);

};
#endif
