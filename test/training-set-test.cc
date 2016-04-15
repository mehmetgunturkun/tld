#include <cstdlib>
#include <cstdio>

#include "core/Box.hpp"
#include "detector/common/ScoredBox.hpp"
#include "detector/training/TrainingSet.hpp"
#include "detector/ensemble/EnsembleClassifier.hpp"
#include "detector/nn/NearestNeighborClassifier.hpp"
#include "detector/CascadedSingleDetector.hpp"
#include "util/Dataset.hpp"
#include "util/Util.h"
#include "core/FrameView.hpp"
#include "util/Random.hpp"

class MockedEnsembleClassifier: public EnsembleClassifier {
    void init(TrainingSet<Box> ts) {
        vector<Box*> positiveSamples = ts.positiveSamples;
        for (int i = 0; i < ts.nrOfPositiveSamples; i++) {
            FrameView* fv = new FrameView(ts.frame);
            fv->addBox(positiveSamples[i], FrameView::RED);
            Image::imshow("pos", fv->underlying, 100);
        }

        vector<Box*> negativeSamples = ts.negativeSamples;
        for (int i = 0; i < ts.nrOfNegativeSamples; i++) {
            FrameView* fv = new FrameView(ts.frame);
            fv->addBox(negativeSamples[i], FrameView::RED);
            Image::imshow("neg", fv->underlying, 100);
        }
    }
};

class MockedNearestNeighborClassifier: public NearestNeighborClassifier {
    void init(TrainingSet<Box> ts) {
        vector<Box*> positiveSamples = ts.positiveSamples;
        for (int i = 0; i < ts.nrOfPositiveSamples; i++) {
            FrameView* fv = new FrameView(ts.frame);
            fv->addBox(positiveSamples[i], FrameView::RED);
            Image::imshow("pos", fv->underlying, 0);
        }

        vector<Box*> negativeSamples = ts.negativeSamples;
        for (int i = 0; i < ts.nrOfNegativeSamples; i++) {
            FrameView* fv = new FrameView(ts.frame);
            fv->addBox(negativeSamples[i], FrameView::RED);
            Image::imshow("neg", fv->underlying, 100);
        }
    }
};

int main(int argc, char** argv) {
    Random::init_genrand(100);
    Dataset* car = new Dataset("car");

    Frame* firstFrame = car->next();
    Box* firstBox = car->initBox;

    EnsembleClassifier* mockedEnsembleClassifier = (EnsembleClassifier*) new MockedEnsembleClassifier();
    NearestNeighborClassifier* mockedNearestNeighborClassifier = (NearestNeighborClassifier*) new MockedNearestNeighborClassifier();

    CascadedSingleDetector* stubbedDetector = new CascadedSingleDetector(mockedEnsembleClassifier, mockedNearestNeighborClassifier);
    stubbedDetector->init(firstFrame, firstBox);

    return EXIT_SUCCESS;
}
