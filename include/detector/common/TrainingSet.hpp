#ifndef TRAINING_SET_H
#define TRAINING_SET_H
#include <stddef.h>
#include <vector>
using namespace std;

template <typename Type>

class TrainingSet {
public:
    int nrOfPositiveSamples;
    vector<Type*> positiveSamples;

    int nrOfNegativeSamples;
    vector<Type*> negativeSamples;
    TrainingSet(vector<Type*> p, vector<Type*> n) {
        positiveSamples = p;
        nrOfPositiveSamples = (int) p.size();

        negativeSamples = n;
        nrOfNegativeSamples = (int) n.size();
    }
};
#endif
