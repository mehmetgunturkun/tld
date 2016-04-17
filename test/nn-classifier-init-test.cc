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

class MockedEnsembleClassifier: public EnsembleClassifier {
    void init(TrainingSet<Box> ts) {}
};


int main(int argc, char** argv) {
    Conf::load("resources/tld.json");
    Dataset* car = new Dataset("car");

    Frame* firstFrame = car->next();
    Box* firstBox = car->initBox;

    EnsembleClassifier* mockedEnsembleClassifier = (EnsembleClassifier*) new MockedEnsembleClassifier();
    NearestNeighborClassifier* nearestNeighborClassifier = new NearestNeighborClassifier();

    CascadedSingleDetector* stubbedDetector = new CascadedSingleDetector(mockedEnsembleClassifier, nearestNeighborClassifier);
    stubbedDetector->init(firstFrame, firstBox);

    nearestNeighborClassifier->dumpObjectModel();
    return EXIT_SUCCESS;
}
