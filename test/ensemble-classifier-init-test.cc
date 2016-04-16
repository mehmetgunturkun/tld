#include <cstdlib>
#include <cstdio>
#include "core/Frame.hpp"
#include "core/FrameView.hpp"
#include "core/Box.hpp"
#include "detector/common/ScoredBox.hpp"
#include "detector/training/TrainingSet.hpp"
#include "detector/ensemble/EnsembleClassifier.hpp"
#include "detector/nn/NearestNeighborClassifier.hpp"
#include "detector/CascadedSingleDetector.hpp"

#include "util/Util.h"
#include "util/Dataset.hpp"

class MockedNearestNeighborClassifier: public NearestNeighborClassifier {
    void init(TrainingSet<Box> ts) {
        // vector<Box*> positiveSamples = ts.positiveSamples;
        // for (int i = 0; i < ts.nrOfPositiveSamples; i++) {
        //     FrameView* fv = new FrameView(ts.frame);
        //     fv->addBox(positiveSamples[i], FrameView::RED);
        //     Image::imshow("pos", fv->underlying, 0);
        // }
        //
        // vector<Box*> negativeSamples = ts.negativeSamples;
        // for (int i = 0; i < ts.nrOfNegativeSamples; i++) {
        //     FrameView* fv = new FrameView(ts.frame);
        //     fv->addBox(negativeSamples[i], FrameView::RED);
        //     Image::imshow("neg", fv->underlying, 100);
        // }
    }
};

int main(int argc, char** argv) {
    Conf::load("resources/tld.json");
    Dataset* car = new Dataset("car");

    Frame* firstFrame = car->next();
    Box* firstBox = car->initBox;

    EnsembleClassifier* eClassifier = new EnsembleClassifier();
    NearestNeighborClassifier* mockedNearestNeighborClassifier = (NearestNeighborClassifier*) new MockedNearestNeighborClassifier();

    CascadedSingleDetector* stubbedDetector = new CascadedSingleDetector(eClassifier, mockedNearestNeighborClassifier);
    stubbedDetector->init(firstFrame, firstBox);
    
    eClassifier->dumpBaseClassifiers();
    return EXIT_SUCCESS;
}
