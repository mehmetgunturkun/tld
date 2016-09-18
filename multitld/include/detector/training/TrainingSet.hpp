#ifndef TRAINING_SET_H
#define TRAINING_SET_H
#include <stddef.h>
#include <vector>

#include "core/Frame.hpp"

using namespace std;

template <typename Type>
class Labelled {
public:
    Frame* frame;
    Type* item;
    int label;
    Labelled(Frame* frame, Type* item, int l) {
        this->frame = frame;
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
    int nrOfBootstrap;
    TrainingSet(Frame* f, vector<Type*> p, vector<Type*> n, int nrOfBootstrap = 1) {
        this->frame = f;
        this->positiveSamples = p;
        this->nrOfPositiveSamples = (int) p.size();

        this->negativeSamples = n;
        this->nrOfNegativeSamples = (int) n.size();

        this->nrOfSamples = nrOfPositiveSamples + nrOfNegativeSamples;
        this->nrOfBootstrap = nrOfBootstrap;
    }

    vector<Labelled<Type>> getLabelledSamples(bool shouldWarp = false) {
        vector<Labelled<Type>> labelledSamples;
        Mat* gaussian = frame->gaussian;

        Mat* img = gaussian;
        Frame* f = frame;

        for (int i = 0; i < nrOfNegativeSamples; i++) {
            Type* sample = negativeSamples[i];
            Labelled<Type> negativeSample = Labelled<Type>(f, sample, 0);
            labelledSamples.push_back(negativeSample);
        }

        int warpNo = 0;
        int warpLimit = 1;
        if (shouldWarp) {
            warpLimit = 10;
        }

        do {
            for (int i = 0; i < nrOfPositiveSamples; i++) {
                Type* sample = positiveSamples[i];
                Labelled<Type> positiveSample = Labelled<Type>(f, sample, 1);
                labelledSamples.push_back(positiveSample);
            }

            img = Image::warp(gaussian);
            f = new Frame(img, f->grayscale);
            warpNo += 1;
        } while (warpNo < warpLimit);
        std::random_shuffle(labelledSamples.begin(), labelledSamples.end());
        return labelledSamples;
    }
};
#endif
