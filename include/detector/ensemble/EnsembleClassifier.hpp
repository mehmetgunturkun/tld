#ifndef ENSEMBLE_CLASSIFIER_H
#define ENSEMBLE_CLASSIFIER_H

#include "util/Random.hpp"
#include "core/Frame.hpp"
#include "core/Box.hpp"
#include "detector/ScoredBox.hpp"

#include "detector/DetectResult.hpp"
#include "detector/ensemble/PixelComparison.hpp"
#include "detector/ensemble/BaseClassifier.hpp"

using namespace std;

class EnsembleClassifier {
private:
    vector<BaseClassifier*> baseClassifiers;
    int nrOfBaseClassifiers;
    int nrOfPixelComparisons;
    
    double minimumPositiveThreshold;
    string classifierName;
    vector<BaseClassifier*> generateBaseClassifier();
    vector<PixelComparison*> produceAllComparisons();
    vector<BaseClassifier*> shuffleComparisons(vector<PixelComparison*> allComparisons);
public:
    EnsembleClassifier();
    void init(Frame* frame, Box* box);
    bool classify(Frame* frame, ScoredBox* box);
    void update(Frame* frame, Box* box, DetectResult* detectResult);
};

#endif
