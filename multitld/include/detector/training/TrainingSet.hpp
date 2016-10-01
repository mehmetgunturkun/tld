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
    string title;
    Frame* frame;
    int nrOfPositiveSamples;
    vector<Type*> positiveSamples;

    int nrOfNegativeSamples;
    vector<Type*> negativeSamples;

    int nrOfSamples;
    int nrOfBootstrap;

    TrainingSet(Frame* f, vector<Type*> p, vector<Type*> n, int nrOfBootstrap = 1) {
        construct(f, p, n, nrOfBootstrap);
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

    vector<Labelled<Type>*> getShuffledSamples() {
        vector<Labelled<Type>*> labelledSamples;

        for (int i = 0; i < nrOfPositiveSamples; i++) {
            Type* sample = positiveSamples[i];
            Labelled<Type>* positiveSample = new Labelled<Type>(frame, sample, 1);
            labelledSamples.push_back(positiveSample);
        }

        for (int i = 0; i < nrOfNegativeSamples; i++) {
            Type* sample = negativeSamples[i];
            Labelled<Type>* negativeSample = new Labelled<Type>(frame, sample, 0);
            labelledSamples.push_back(negativeSample);
        }
        Random::seed();
        labelledSamples = Random::randomSample(labelledSamples);
        return labelledSamples;
    };


    vector<Labelled<Type>*> getPositiveFirstSamples() {
        vector<Labelled<Type>*> labelledSamples;

        for (int i = 0; i < nrOfPositiveSamples; i++) {
            Type* sample = positiveSamples[i];
            Labelled<Type>* positiveSample = new Labelled<Type>(frame, sample, 1);
            labelledSamples.push_back(positiveSample);
        }

        for (int i = 0; i < nrOfNegativeSamples; i++) {
            Type* sample = negativeSamples[i];
            Labelled<Type>* negativeSample = new Labelled<Type>(frame, sample, 0);
            labelledSamples.push_back(negativeSample);
        }
        Random::seed();
        labelledSamples = Random::randomSample(labelledSamples);

        if (nrOfPositiveSamples > 0) {
            Type* sample = positiveSamples[0];
            Labelled<Type>* positiveSample = new Labelled<Type>(frame, sample, 1);
            labelledSamples.insert(labelledSamples.begin(), positiveSample);
        }

        return labelledSamples;
    };

    vector<Labelled<Type>*> getLabelledSamples(bool shouldWarp = false,
        bool useGaussian = true,
        bool positiveFirst = false,
        bool splitData = false) {
            vector<Labelled<Type>*> labelledSamples;
            Mat* gaussian;
            if (useGaussian) {
                gaussian = frame->gaussian;
            } else {
                gaussian = frame->grayscale;
            }

            Mat* img = gaussian;
            Frame* f = frame;

            if (splitData) {
                negativeSamples = Random::randomSample(negativeSamples, nrOfNegativeSamples / 2);
                nrOfNegativeSamples = (int) negativeSamples.size();
            }

            for (int i = 0; i < nrOfNegativeSamples; i++) {
                Type* sample = negativeSamples[i];
                Labelled<Type>* negativeSample = new Labelled<Type>(f, sample, 0);
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
                    Labelled<Type>* positiveSample = new Labelled<Type>(f, sample, 1);
                    labelledSamples.push_back(positiveSample);
                }

                img = Image::warp(gaussian);
                f = new Frame(img, f->grayscale);
                warpNo += 1;
            } while (warpNo < warpLimit);

            std::random_shuffle(labelledSamples.begin(), labelledSamples.end());
            if (positiveFirst) {
                for (int i = 0; i < nrOfPositiveSamples; i++) {
                    Type* sample = positiveSamples[i];
                    Labelled<Type>* positiveSample = new Labelled<Type>(f, sample, 1);
                    labelledSamples.insert(labelledSamples.begin(), positiveSample);
                }
            }
            return labelledSamples;
    }
};
#endif
