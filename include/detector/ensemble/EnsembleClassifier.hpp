#ifndef ENSEMBLE_CLASSIFIER_H
#define ENSEMBLE_CLASSIFIER_H

#include "util/Random.hpp"
#include "common/Config.hpp"
#include "core/Frame.hpp"
#include "core/Box.hpp"
#include "detector/common/ScoredBox.hpp"

#include "detector/DetectResult.hpp"
#include "detector/training/TrainingSet.hpp"
#include "detector/ensemble/EnsembleClassificationDetails.hpp"
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
    virtual void init(TrainingSet<Box> ts);
    bool classify(Frame* frame, ScoredBox* box);
    EnsembleClassificationDetails* score(Frame* frame, Box* box);
    void update(TrainingSet<ScoredBox> ts);
    string toString() {
        stringstream ss;
        for (int i = 0; i < nrOfBaseClassifiers; i++) {
            BaseClassifier* bc = baseClassifiers[i];
            ss << bc->toString()
               << "\n";
        }
        return ss.str();
    }
};

#endif
