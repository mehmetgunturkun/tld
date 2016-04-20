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

int main(int argc, char** argv) {
    Conf::load("resources/tld.json");
    Dataset* car = new Dataset("car");

    Frame* firstFrame = car->next();
    Box* firstBox = car->initBox;

    EnsembleClassifier* eClassifier = new EnsembleClassifier();
    NearestNeighborClassifier* nnClassifier = new NearestNeighborClassifier();

    CascadedSingleDetector* stubbedDetector = new CascadedSingleDetector(eClassifier, nnClassifier);
    stubbedDetector->init(firstFrame, firstBox);

    VarianceClassifier* vClassifier = new VarianceClassifier(firstFrame, firstBox);

    while (car->hasNext()) {
        Frame* nextFrame = car->next();
        BoxIterator* iterator = new BoxIterator(nextFrame, firstBox, 10, 24);

        while (iterator->hasNext()) {
            Box* nextBox = iterator->next();
            if (!vClassifier->classify(nextFrame, nextBox)) {
                continue;
            }

            ScoredBox* scoredBox = new ScoredBox(nextBox);
            if (!eClassifier->classify(nextFrame, scoredBox)) {
                continue;
            }

            if (nnClassifier->classify(nextFrame, scoredBox)) {
                println("Score: %g", scoredBox->getScore("nn"));
                FrameView* view = new FrameView(nextFrame);
                Box* nextBox = iterator->next();
                view->addBox(nextBox, FrameView::RED);
                Image::imshow("ensemble", view->underlying, 5);
            }
        }
    }
    return EXIT_SUCCESS;
}
