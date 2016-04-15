#ifndef TRAINING_SET_H
#define TRAINING_SET_H
#include <stddef.h>
#include <vector>
#include "util/Util.h"
#include "core/Frame.hpp"

using namespace std;

template <typename Type>

class TrainingSet {
public:
    Frame* frame;
    int nrOfPositiveSamples;
    vector<Type*> positiveSamples;

    int nrOfNegativeSamples;
    vector<Type*> negativeSamples;
    TrainingSet(Frame* f, vector<Type*> p, vector<Type*> n) {
        frame = f;
        positiveSamples = p;
        nrOfPositiveSamples = (int) p.size();

        negativeSamples = n;
        nrOfNegativeSamples = (int) n.size();
    }
};
#endif
