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
    void init(TrainingSet<Box> ts) {}
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

    ScoredBox* scoredBox = new ScoredBox(firstBox);
    eClassifier->classify(firstFrame, scoredBox);
    println("------------------");
    double score = scoredBox->getScore("ensemble");
    println("Score: %g", score);
    return EXIT_SUCCESS;
}
