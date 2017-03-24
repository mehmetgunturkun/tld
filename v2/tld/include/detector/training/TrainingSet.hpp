#ifndef TRAINING_SET_H
#define TRAINING_SET_H
#include <stddef.h>
#include <vector>

#include "core/Frame.hpp"

using namespace std;

template <typename Type>
class Labelled {
public:
    Type* item;
    int label;
    Labelled(Type* item, int label) {
        this->item = item;
        this->label = label;
    }

    ~Labelled() {
        delete item;
    }
};

template <typename Type>
class TrainingSet {
public:
    string title;
    Frame* frame;
    int nrOfPositiveSamples;
    vector<Type*> positiveSamples;

    int nrOfNegativeSamples;
    vector<Type*> negativeSamples;

    int nrOfSamples;
    int nrOfBootstrap;

    bool shouldShuffle;

    TrainingSet(Frame* f, vector<Type*> p, vector<Type*> n, int nrOfBootstrap = 1, bool shuffle = false) {
        construct(f, p, n, nrOfBootstrap);
        this->shouldShuffle = shuffle;
    }

    void construct(Frame* f, vector<Type*> p, vector<Type*> n, int nrOfBootstrap = 1) {
        this->frame = f;
        this->positiveSamples = p;
        this->nrOfPositiveSamples = (int) p.size();

        this->negativeSamples = n;
        this->nrOfNegativeSamples = (int) n.size();

        this->nrOfSamples = nrOfPositiveSamples + nrOfNegativeSamples;
        this->nrOfBootstrap = nrOfBootstrap;
    }
};
#endif
