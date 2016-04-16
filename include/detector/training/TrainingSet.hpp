#ifndef TRAINING_SET_H
#define TRAINING_SET_H
#include <stddef.h>
#include <vector>
#include "util/Util.h"
#include "core/Frame.hpp"

using namespace std;

template <typename Type>
class Labelled {
public:
    Type* item;
    int label;
    Labelled(Type* item, int l) {
        this->item = item;
        this->label = l;
    }
};

template <typename Type>
class TrainingSet {
public:
    Frame* frame;
    int nrOfPositiveSamples;
    vector<Type*> positiveSamples;

    int nrOfNegativeSamples;
    vector<Type*> negativeSamples;

    int nrOfSamples;
    TrainingSet(Frame* f, vector<Type*> p, vector<Type*> n) {
        frame = f;
        positiveSamples = p;
        nrOfPositiveSamples = (int) p.size();

        negativeSamples = n;
        nrOfNegativeSamples = (int) n.size();

        nrOfSamples = nrOfPositiveSamples + nrOfNegativeSamples;
    }

    vector<Labelled<Type>> getLabelledSamples() {
        vector<Labelled<Type>> labelledSamples;
        for (int i = 0; i < nrOfPositiveSamples; i++) {
            Type* sample = positiveSamples[i];
            Labelled<Type> positiveSample = Labelled<Type>(sample, 1);
            labelledSamples.push_back(positiveSample);
        }

        for (int i = 0; i < nrOfNegativeSamples; i++) {
            Type* sample = negativeSamples[i];
            Labelled<Type> negativeSample = Labelled<Type>(sample, 0);
            labelledSamples.push_back(negativeSample);
        }
        return labelledSamples;
    }
};
#endif
