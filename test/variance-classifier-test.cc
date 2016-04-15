#include <cstdlib>
#include <cstdio>
#include "core/Frame.hpp"
#include "core/FrameView.hpp"
#include "core/Box.hpp"
#include "detector/common/BoxIterator.hpp"
#include "detector/variance/VarianceClassifier.hpp"

#include "util/Util.h"
#include "util/Dataset.hpp"

int main(int argc, char** argv) {
    Dataset* dataset = new Dataset("car");
    Frame* f = dataset->next();
    println(f->toString().c_str());

    Box* b = dataset->initBox;
    println(b->toString().c_str());

    VarianceClassifier* vClassifier = new VarianceClassifier(f, b);
    BoxIterator* iterator = new BoxIterator(f, b, 10, 24);

    while (iterator->hasNext()) {
        Box* nextBox = iterator->next();
        if (vClassifier->classify(f, nextBox)) {
            FrameView* view = new FrameView(f);
            Box* nextBox = iterator->next();
            view->addBox(b, FrameView::BLUE);
            view->addBox(nextBox, FrameView::RED);
            Image::imshow("iterator", view->underlying, 1);
        }
    }
    return EXIT_SUCCESS;
}
